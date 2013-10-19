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

/** \file
 * Functionals for domain initialization -- header file.
 */
#ifndef DYNAMICS_PROCESSOR_2D_H
#define DYNAMICS_PROCESSOR_2D_H

#include "core/globalDefs.h"
#include "atomicBlock/dataProcessingFunctional2D.h"
#include "core/dynamics.h"

namespace plb {

/* *************** Class ExternalRhoJcollideAndStream2D ******************* */

template<typename T, template<typename U> class Descriptor>
class ExternalRhoJcollideAndStream2D : public BoxProcessingFunctional2D
{
public:
    // Block 0: lattice; Block 1: rhoBar; Block 2: j.
    virtual void processGenericBlocks( Box2D domain,
                                       std::vector<AtomicBlock2D*> atomicBlocks );
    virtual ExternalRhoJcollideAndStream2D<T,Descriptor>* clone() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
private:
    void collide (
            BlockLattice2D<T,Descriptor>& lattice, Box2D const& domain,
            ScalarField2D<T> const& rhoBarField, Dot2D const& offset1,
            TensorField2D<T,2> const& jField, Dot2D const& offset2, BlockStatistics& stat );
    void bulkCollideAndStream (
            BlockLattice2D<T,Descriptor>& lattice, Box2D const& domain,
            ScalarField2D<T> const& rhoBarField, Dot2D const& offset1,
            TensorField2D<T,2> const& jField, Dot2D const& offset2, BlockStatistics& stat );
    void boundaryStream (
            BlockLattice2D<T,Descriptor>& lattice,
            Box2D const& bound, Box2D const& domain );
};


template<typename T, template<typename U> class Descriptor>
class Tau1CollideAndStream2D : public BoxProcessingFunctional2D_L<T,Descriptor>
{
public:
    virtual void process( Box2D domain, BlockLattice2D<T,Descriptor>& lattice );
    virtual Tau1CollideAndStream2D<T,Descriptor>* clone() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
private:
    void bulkStream (
            Array<T,Descriptor<T>::q> const& f, plint iX, plint iY );
    void boundaryStream (
            Array<T,Descriptor<T>::q> const& f, plint iX, plint iY );
};

}  // namespace plb

#endif  // DYNAMICS_PROCESSOR_2D_H
