// Author: François Faure, INRIA-UJF, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
#ifndef SOFA_COMPONENTS_STIFFSPRINGFORCEFIELD_H
#define SOFA_COMPONENTS_STIFFSPRINGFORCEFIELD_H

#include "Sofa/Components/SpringForceField.h"

namespace Sofa
{

namespace Components
{

template<class DataTypes>
class StiffSpringForceField : public SpringForceField<DataTypes>
{
public:
    typedef SpringForceField<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;
    typedef typename Inherit::Spring Spring;
    class Mat3 : public fixed_array<Deriv,3>
    {
    public:
        Deriv operator*(const Deriv& v)
        {
            return Deriv((*this)[0]*v,(*this)[1]*v,(*this)[2]*v);
        }
    };

protected:
    std::vector<Mat3> dfdx;
    double m_potentialEnergy;

    void addSpringForce(double& potentialEnergy, VecDeriv& f1, const VecCoord& p1, const VecDeriv& v1, VecDeriv& f2, const VecCoord& p2, const VecDeriv& v2, int i, const Spring& spring);

    void addSpringDForce(VecDeriv& f1, const VecCoord& p1, const VecDeriv& dx1, VecDeriv& f2, const VecCoord& p2, const VecDeriv& dx2, int i, const Spring& spring);

public:
    StiffSpringForceField(Core::MechanicalModel<DataTypes>* object1, Core::MechanicalModel<DataTypes>* object2, double ks=100.0, double kd=5.0)
        : SpringForceField<DataTypes>(object1, object2, ks, kd)
    {
    }

    StiffSpringForceField(Core::MechanicalModel<DataTypes>* object, double ks=100.0, double kd=5.0)
        : SpringForceField<DataTypes>(object, ks, kd)
    {
    }

    virtual void init();

    virtual void addForce();

    virtual void addDForce();

    virtual double getPotentialEnergy() { return m_potentialEnergy; }
};

} // namespace Components

} // namespace Sofa

#endif
