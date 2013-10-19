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

#ifndef SERIALIZER_IO_2D_H
#define SERIALIZER_IO_2D_H

#include "core/globalDefs.h"
#include "core/block2D.h"
#include "multiBlock/multiBlock2D.h"
#include "multiBlock/multiDataField2D.h"
#include "multiBlock/multiBlockLattice2D.h"
#include "atomicBlock/dataField2D.h"
#include "atomicBlock/blockLattice2D.h"
#include "io/serializerIO.h"
#include "io/parallelIO.h"

namespace plb {

/// Save the content of a Block2D into a Base64 encoded binary file.
/** The content includes external scalars in the case of a BlockLattice2D. Only raw
 *  data is written, and structural information such as length and width of the
 *  block is lost.
 *
 *  Index-ordering for this operation can be chosen through a call to
 *  global::IOpolicy().setIndexOrderingForStreams(IndexOrdering::OrderingT).
 */
void saveBinaryBlock(Block2D const& block, std::string fName, bool enforceUint=false);

/// Load the content of a Block2D from a Base64 encoded binary file.
/** The content includes external scalars in the case of a BlockLattice2D. Only raw
 *  data is written, and structural information such as length and width of the
 *  block is lost.
 *
 *  Index-ordering for this operation can be chosen through a call to
 *  global::IOpolicy().setIndexOrderingForStreams(IndexOrdering::OrderingT).
 */
void loadBinaryBlock(Block2D& block, std::string fName, bool enforceUint=false);

/// Flush the content of a Block2D into a generic C++ stream with space-separated ASCII words.
/** The content includes external scalars in the case of a BlockLattice2D. Only raw
 *  data is written, and structural information such as length and width of the
 *  block is lost.
 *
 *  Index-ordering for this operation can be chosen through a call to
 *  global::IOpolicy().setIndexOrderingForStreams(IndexOrdering::OrderingT).
 *
 *  This file format is not exact and should be used for data post-processing only,
 *  and not for checkpointing.
 */
template<typename T>
std::ostream& block2ostream(std::ostream& ostr, Block2D const& block);

template<typename T>
std::ostream& operator<<(std::ostream& ostr, MultiScalarField2D<T> const& block) {
    return block2ostream<T>(ostr, block);
}

template<typename T>
std::ostream& operator<<(std::ostream& ostr, MultiNTensorField2D<T> const& block) {
    return block2ostream<T>(ostr, block);
}

template<typename T, int nDim>
std::ostream& operator<<(std::ostream& ostr, MultiTensorField2D<T,nDim> const& block) {
    return block2ostream<T>(ostr, block);
}

template<typename T, template<typename U> class Descriptor>
std::ostream& operator<<(std::ostream& ostr, MultiBlockLattice2D<T,Descriptor> const& block) {
    return block2ostream<T>(ostr, block);
}



template<typename T>
std::ostream& operator<<(std::ostream& ostr, ScalarField2D<T> const& block) {
    return block2ostream<T>(ostr, block);
}

template<typename T>
std::ostream& operator<<(std::ostream& ostr, NTensorField2D<T> const& block) {
    return block2ostream<T>(ostr, block);
}

template<typename T, int nDim>
std::ostream& operator<<(std::ostream& ostr, TensorField2D<T,nDim> const& block) {
    return block2ostream<T>(ostr, block);
}

template<typename T, template<typename U> class Descriptor>
std::ostream& operator<<(std::ostream& ostr, BlockLattice2D<T,Descriptor> const& block) {
    return block2ostream<T>(ostr, block);
}


/// Flush the content of a generic C++ stream with ASCII content into a Block2D.
/** The content includes external scalars in the case of a BlockLattice2D.
 *
 *  Index-ordering for this operation can be chosen through a call to
 *  global::IOpolicy().setIndexOrderingForStreams(IndexOrdering::OrderingT).
 *
 *  This file format is not exact and should be used for data post-processing only,
 *  and not for checkpointing.
 */
template<typename T>
std::istream& istream2block(std::istream& istr, Block2D& block);

template<typename T>
std::istream& operator>>(std::istream& istr, MultiScalarField2D<T>& block) {
    return istream2block<T>(istr, block);
}

template<typename T>
std::istream& operator>>(std::istream& istr, MultiNTensorField2D<T>& block) {
    return istream2block<T>(istr, block);
}

template<typename T,int nDim>
std::istream& operator>>(std::istream& istr, MultiTensorField2D<T,nDim>& block) {
    return istream2block<T>(istr, block);
}

template<typename T, template<typename U> class Descriptor>
std::istream& operator>>(std::istream& istr, MultiBlockLattice2D<T,Descriptor>& block) {
    return istream2block<T>(istr, block);
}


template<typename T>
std::istream& operator>>(std::istream& istr, ScalarField2D<T>& block) {
    return istream2block<T>(istr, block);
}

template<typename T>
std::istream& operator>>(std::istream& istr, NTensorField2D<T>& block) {
    return istream2block<T>(istr, block);
}

template<typename T,int nDim>
std::istream& operator>>(std::istream& istr, TensorField2D<T,nDim>& block) {
    return istream2block<T>(istr, block);
}

template<typename T, template<typename U> class Descriptor>
std::istream& operator>>(std::istream& istr, BlockLattice2D<T,Descriptor>& block) {
    return istream2block<T>(istr, block);
}

} // namespace plb

#endif  // SERIALIZER_IO_2D_H
