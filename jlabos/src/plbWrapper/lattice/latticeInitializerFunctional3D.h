/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2012 FlowKit Sarl
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
#ifndef SWIG_LATTICE_INITIALIZER_FUNCTIONAL_3D_H
#define SWIG_LATTICE_INITIALIZER_FUNCTIONAL_3D_H

#include "core/globalDefs.h"
#include "atomicBlock/dataProcessingFunctional3D.h"
#include "core/dynamics.h"
#include <vector>

namespace plb {


template<typename T, template<typename U> class Descriptor>
class MaskedIniDynamicsFunctional3D : public MaskedBoxProcessingFunctional3D_L<T,Descriptor> {
public:
    MaskedIniDynamicsFunctional3D(Dynamics<T,Descriptor>* dynamics_);
    MaskedIniDynamicsFunctional3D(MaskedIniDynamicsFunctional3D<T,Descriptor> const& rhs);
    MaskedIniDynamicsFunctional3D<T,Descriptor>& operator= (
            MaskedIniDynamicsFunctional3D<T,Descriptor> const& rhs );
    ~MaskedIniDynamicsFunctional3D();
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice,
                          NTensorField3D<int>& mask );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual MaskedIniDynamicsFunctional3D<T,Descriptor>* clone() const;
private:
    Dynamics<T,Descriptor>* dynamics;
};

template<typename T, template<typename U> class Descriptor>
class N_IniBoundaryVelocityFunctional3D : public BoxProcessingFunctional3D_LN<T,Descriptor,T> {
public:
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice,
                          NTensorField3D<T>& velocity );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual N_IniBoundaryVelocityFunctional3D<T,Descriptor>* clone() const;
};

template<typename T, template<typename U> class Descriptor>
class Masked_N_IniBoundaryVelocityFunctional3D : public MaskedBoxProcessingFunctional3D_LN<T,Descriptor,T> {
public:
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice,
                          NTensorField3D<T>& velocity,
                          NTensorField3D<int>& mask );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual Masked_N_IniBoundaryVelocityFunctional3D<T,Descriptor>* clone() const;
};

template<typename T, template<typename U> class Descriptor>
class N_IniEquilibriumFunctional3D : public BoxProcessingFunctional3D {
public:
    virtual void processGenericBlocks (
            Box3D domain, std::vector<AtomicBlock3D*> blocks);
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual N_IniEquilibriumFunctional3D<T,Descriptor>* clone() const;
};

template<typename T, template<typename U> class Descriptor>
class Masked_N_IniEquilibriumFunctional3D : public BoxProcessingFunctional3D {
public:
    virtual void processGenericBlocks (
            Box3D domain, std::vector<AtomicBlock3D*> blocks);
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual Masked_N_IniEquilibriumFunctional3D<T,Descriptor>* clone() const;
};

template<typename T, template<typename U> class Descriptor>
class N_IniPopulationsFunctional3D : public BoxProcessingFunctional3D_LN<T,Descriptor,T> {
public:
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice,
                          NTensorField3D<T>& populations );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual N_IniPopulationsFunctional3D<T,Descriptor>* clone() const;
};

template<typename T, template<typename U> class Descriptor>
class Masked_N_IniPopulationsFunctional3D : public MaskedBoxProcessingFunctional3D_LN<T,Descriptor,T> {
public:
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice,
                          NTensorField3D<T>& populations,
                          NTensorField3D<int>& mask );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual Masked_N_IniPopulationsFunctional3D<T,Descriptor>* clone() const;
};

template<typename T, template<typename U> class Descriptor>
class N_IniConstPopulationsFunctional3D : public BoxProcessingFunctional3D_L<T,Descriptor> {
public:
    N_IniConstPopulationsFunctional3D(std::vector<T> const& pop_);
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual N_IniConstPopulationsFunctional3D<T,Descriptor>* clone() const;
private:
    std::vector<T> pop;
};

template<typename T, template<typename U> class Descriptor>
class Masked_N_IniConstPopulationsFunctional3D : public MaskedBoxProcessingFunctional3D_L<T,Descriptor> {
public:
    Masked_N_IniConstPopulationsFunctional3D(std::vector<T> const& pop_);
    virtual void process (
            Box3D domain, BlockLattice3D<T,Descriptor>& lattice,
            NTensorField3D<int>& mask );
    virtual BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    virtual Masked_N_IniConstPopulationsFunctional3D<T,Descriptor>* clone() const;
private:
    std::vector<T> pop;
};

}  // namespace plb

#endif  // SWIG_LATTICE_INITIALIZER_FUNCTIONAL_3D_H
