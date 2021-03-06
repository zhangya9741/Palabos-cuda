/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2013 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OFF_LATTICE_BOUNDARY_PROCESSOR_2D_HH
#define OFF_LATTICE_BOUNDARY_PROCESSOR_2D_HH

#include "offLattice/offLatticeBoundaryProcessor2D.h"
#include "offLattice/nextNeighbors2D.h"
#include <algorithm>
#include <cmath>

namespace plb
{

template<typename T>
CheckVoxelizationFunctional2D<T>::
CheckVoxelizationFunctional2D (
    BoundaryShape2D<T,Array<T,3> >* shape_, int flowType_ )
    : numErrorsId ( this->getStatistics().subscribeIntSum() ),
      shape ( shape_ ),
      flowType ( flowType_ )
{ }

template<typename T>
CheckVoxelizationFunctional2D<T>::~CheckVoxelizationFunctional2D()
{
    delete shape;
}

template<typename T>
CheckVoxelizationFunctional2D<T>::
CheckVoxelizationFunctional2D (
    CheckVoxelizationFunctional2D<T> const& rhs )
    : PlainReductiveBoxProcessingFunctional2D ( rhs ),
      numErrorsId ( rhs.numErrorsId ),
      shape ( rhs.shape->clone() ),
      flowType ( rhs.flowType )
{ }

template<typename T>
CheckVoxelizationFunctional2D<T>&
CheckVoxelizationFunctional2D<T>::operator= (
    CheckVoxelizationFunctional2D<T> const& rhs )
{
    PlainReductiveBoxProcessingFunctional2D::operator= ( rhs );
    numErrorsId = rhs.numErrorsId;
    shape = rhs.shape->clone();
    flowType = rhs.flowType;
    return *this;
}


template<typename T>
CheckVoxelizationFunctional2D<T>*
CheckVoxelizationFunctional2D<T>::clone() const
{
    return new CheckVoxelizationFunctional2D<T> ( *this );
}

template<typename T>
void CheckVoxelizationFunctional2D<T>::getTypeOfModification (
    std::vector<modif::ModifT>& modified ) const
{
    modified[0] = modif::staticVariables;  // Flag matrix.
    // Possible additional parameters for the shape function are read-only.
    for ( pluint i=1; i<modified.size(); ++i )
    {
        modified[i] = modif::nothing;
    }
}

template<typename T>
BlockDomain::DomainT CheckVoxelizationFunctional2D<T>::appliesTo() const
{
    return BlockDomain::bulk;
}

template<typename T>
bool CheckVoxelizationFunctional2D<T>::isFluid ( Dot2D const& location ) const
{
    if ( flowType==voxelFlag::inside )
    {
        return shape->isInside ( location );
    }
    else
    {
        return !shape->isInside ( location );
    }
}

template<typename T>
void CheckVoxelizationFunctional2D<T>::processGenericBlocks (
    Box2D domain, std::vector<AtomicBlock2D*> fields )
{
    PLB_PRECONDITION ( fields.size() >= 1 );
    ScalarField2D<int>* flagMatrix =
        dynamic_cast<ScalarField2D<int>*> ( fields[0] );
    PLB_ASSERT ( flagMatrix );
    Dot2D absoluteOffset = flagMatrix->getLocation();

    BoundaryShape2D<T,Array<T,2> >* newShape=0;
    if ( fields.size() >1 )
    {
        std::vector<AtomicBlock2D*> shapeParameters ( fields.size()-1 );
        for ( pluint i=0; i<shapeParameters.size(); ++i )
        {
            shapeParameters[i] = fields[i+1];
        }
        newShape=shape->clone ( shapeParameters );
        std::swap ( shape,newShape );
    }

    for ( plint iX=domain.x0; iX<=domain.x1; ++iX )
    {
        for ( plint iY=domain.y0; iY<=domain.y1; ++iY )
        {
            computeCell ( Dot2D ( iX,iY ), *flagMatrix, absoluteOffset );
        }
    }

    if ( newShape )
    {
        std::swap ( shape,newShape );
        delete newShape;
    }
}


template<typename T>
void CheckVoxelizationFunctional2D<T>::computeCell (
    Dot2D const& cellLocation,
    ScalarField2D<int>& flagMatrix,
    Dot2D const& offset )
{
    //  Non-Fluid nodes.
    if ( !isFluid ( cellLocation+offset ) )
    {
        plint numNeighbors=0;
        plint numShallow=0;
        plint numFailures=0;
        for ( int iNeighbor=0; iNeighbor<NextNeighbor2D<T>::numNeighbors; ++iNeighbor )
        {
            int const* c = NextNeighbor2D<T>::c[iNeighbor];
            Dot2D neighbor ( cellLocation.x+c[0], cellLocation.y+c[1] );
            Dot2D nextNeighbor ( cellLocation.x+2*c[0], cellLocation.y+2*c[1] );
            // If the non-fluid node has a fluid neighbor ...
            if ( isFluid ( neighbor+offset ) )
            {
                ++numNeighbors;
                Array<T,3> wallNode;
                T wallDistance;
                Array<T,3> wall_u, wallNormal;
                OffBoundary::Type bdType;
                plint id=-1; // No optimization.
                bool ok = shape->pointOnSurface ( cellLocation+offset, Dot2D ( c[0],c[1] ),
                                                  wallNode, wallDistance, wallNormal,
                                                  wall_u, bdType, id );
                if ( !ok )
                {
                    ++numFailures;
                }
                if ( !isFluid ( nextNeighbor+offset ) )
                {
                    ++numShallow;
                }
            }
        }
        if ( numFailures>0 )
        {
            this->getStatistics().gatherIntSum ( numErrorsId, 1 );
            flagMatrix.get ( cellLocation.x,cellLocation.y ) = 1;
        }
        if ( numNeighbors>0 && numNeighbors==numShallow )
        {
            flagMatrix.get ( cellLocation.x,cellLocation.y ) = 2;
        }
    }
}

template<typename T>
plint CheckVoxelizationFunctional2D<T>::getNumErrors() const
{
    return this->getStatistics().getIntSum ( numErrorsId );
}

}  // namespace plb

#endif  // OFF_LATTICE_BOUNDARY_PROCESSOR_2D_H
