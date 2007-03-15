/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#ifndef SOFA_COMPONENT_COLLISION_CUBEMODEL_H
#define SOFA_COMPONENT_COLLISION_CUBEMODEL_H

#include <sofa/core/CollisionModel.h>
#include <sofa/core/VisualModel.h>
#include <sofa/component/MechanicalObject.h>
#include <sofa/defaulttype/Vec3Types.h>

namespace sofa
{

namespace component
{

namespace collision
{

using namespace sofa::defaulttype;

class CubeModel;

class Cube : public core::TCollisionElementIterator<CubeModel>
{
public:
    Cube(CubeModel* model=NULL, int index=0);

    explicit Cube(const core::CollisionElementIterator& i);

    const Vector3& minVect() const;

    const Vector3& maxVect() const;

    const std::pair<Cube,Cube>& subcells() const;
};

class CubeModel : public core::CollisionModel, public core::VisualModel
{
protected:

    struct CubeData
    {
        Vector3 minBBox, maxBBox;
        std::pair<Cube,Cube> subcells;
        core::CollisionElementIterator leaf; ///< Note that leaf is only meaningfull if subcells in empty
    };

    class CubeSortPredicate;

    std::vector<CubeData> elems;
    std::vector<int> parentOf; ///< Given the index of a child leaf element, store the index of the parent cube

public:
    typedef core::CollisionElementIterator ChildIterator;
    typedef Vec3Types DataTypes;
    typedef Cube Element;
    friend class Cube;

    CubeModel();

    virtual void resize(int size);

    void setParentOf(int childIndex, const Vector3& min, const Vector3& max);

    // -- CollisionModel interface

    virtual void computeBoundingTree(int maxDepth=0);

    virtual std::pair<core::CollisionElementIterator,core::CollisionElementIterator> getInternalChildren(int index) const;

    virtual std::pair<core::CollisionElementIterator,core::CollisionElementIterator> getExternalChildren(int index) const;

    virtual bool isLeaf( int index ) const;

    void draw(int index);

    // -- VisualModel interface

    void draw();

    void initTextures() { }

    void update() { }

protected:

    int addCube(Cube subcellsBegin, Cube subcellsEnd);
    void updateCube(int index);
    void updateCubes();
};

inline Cube::Cube(CubeModel* model, int index)
    : core::TCollisionElementIterator<CubeModel>(model, index)
{}

inline Cube::Cube(const core::CollisionElementIterator& i)
    : core::TCollisionElementIterator<CubeModel>(static_cast<CubeModel*>(i.getCollisionModel()), i.getIndex())
{
}

inline const Vector3& Cube::minVect() const
{
    return model->elems[index].minBBox;
}

inline const Vector3& Cube::maxVect() const
{
    return model->elems[index].maxBBox;
}


inline const std::pair<Cube,Cube>& Cube::subcells() const
{
    return model->elems[index].subcells;
}

} // namespace collision

} // namespace component

} // namespace sofa

#endif
