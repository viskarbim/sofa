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
#include <sofa/helper/io/MeshTrian.h>
#include <stdlib.h>
#include <iostream>
#include <string>

namespace sofa
{

namespace helper
{

namespace io
{

using namespace sofa::defaulttype;

SOFA_DECL_CLASS(MeshTrian)

Creator<Mesh::Factory,MeshTrian> MeshTrianClass("trian");

void MeshTrian::init (std::string filename)
{
    FILE *f = fopen(filename.c_str(), "r");
    if (f)
    {
        readTrian (f);
        fclose(f);
    }
    else
        std::cerr << "File " << filename << " not found " << std::endl;
}

void MeshTrian::readTrian (FILE* file)
{
    int nbp=0;
    fscanf(file, "%d\n", &nbp);

    vertices.resize(nbp);
    for (int p=0; p<nbp; p++)
    {
        fscanf(file, "%lf %lf %lf\n", &vertices[p][0], &vertices[p][1], &vertices[p][2]);
    }

    int nbf=0;
    fscanf(file, "%d\n", &nbf);

    facets.resize(nbf);
    for (int f=0; f<nbf; f++)
    {
        facets[f].resize(3);
        facets[f][0].resize(3);
        facets[f][1].resize(3);
        facets[f][2].resize(3);
        int dummy = 0;
        fscanf(file, "%d %d %d %d %d %d\n", &facets[f][0][0], &facets[f][0][1], &facets[f][0][2], &dummy, &dummy, &dummy);
    }

    // announce the model statistics
    std::cout << " Vertices: " << vertices.size() << std::endl;
    std::cout << " Normals: " << normals.size() << std::endl;
    std::cout << " Texcoords: " << texCoords.size() << std::endl;
    std::cout << " Triangles: " << facets.size() << std::endl;
    if (vertices.size()>0)
    {
        // compute bbox
        Vector3 minBB = vertices[0];
        Vector3 maxBB = vertices[0];
        for (unsigned int i=1; i<vertices.size(); i++)
        {
            Vector3 p = vertices[i];
            for (int c=0; c<3; c++)
            {
                if (minBB[c] > p[c])
                    minBB[c] = p[c];
                if (maxBB[c] < p[c])
                    maxBB[c] = p[c];
            }
        }

        std::cout << "BBox: <"<<minBB[0]<<','<<minBB[1]<<','<<minBB[2]<<">-<"<<maxBB[0]<<','<<maxBB[1]<<','<<maxBB[2]<<">\n";
    }

}

} // namespace io

} // namespace helper

} // namespace sofa

