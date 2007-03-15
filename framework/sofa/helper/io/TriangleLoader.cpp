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
#include <sofa/helper/io/TriangleLoader.h>

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sofa/defaulttype/Vec.h>

namespace sofa
{

namespace helper
{

namespace io
{

using namespace sofa::defaulttype;

// static void skipToEOL(FILE* f)
// {
// 	int	ch;
// 	while ((ch = fgetc(f)) != EOF && ch != '\n');
// }

bool TriangleLoader::load(const char *filename)
{
    FILE*	file;

    /* open the file */
    file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "readOBJ() failed: can't open data file \"%s\".\n",
                filename);
        return false;
    }

    /* announce the model name */
    printf("Loading Triangle model: %s\n", filename);

    loadTriangles (file);
    fclose(file);

    return true;
}

void TriangleLoader::loadTriangles(FILE *file)
{
    assert (file != NULL);

    char buf[128];
    std::vector<Vector3> normals;
    Vector3 n;
    float x, y, z;
//	/* make a default group */
//
    while (fscanf(file, "%s", buf) != EOF)
    {
        switch (buf[0])
        {
        case '#':
            /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':
            /* v, vn, vt */
            switch (buf[1])
            {
            case '\0':
                /* vertex */
                //p = new Vector3();
                fscanf(file, "%f %f %f", &x, &y, &z);
                addVertices(x, y, z);

                // fgets(buf, sizeof(buf), file);
                // numvertices++;
                break;
            case 'n':
                /* normal */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                //fscanf(file, "%lf %lf %lf", &(n.x), &(n.y), &(n.z));
                //normals.push_back(n);
                break;
            case 't':
                /* texcoord */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                break;
            default:
                printf("loadTriangles(): Unknown token \"%s\".\n", buf);
                exit(1);
                break;
            }
            break;
        case 'm':
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'u':
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'g':
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'f':
            /* face */
            fscanf(file, "%s", buf);
            int n1, n2, n3, v1, v2, v3, t1, t2, t3;
            /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
            if (strstr(buf, "//"))
            {
                /* v//n */
                sscanf(buf, "%d//%d", &v1, &n1);
                fscanf(file, "%d//%d", &v2, &n2);
                fscanf(file, "%d//%d", &v3, &n3);
                //Triangle *t = new Triangle(vertices[v1 - 1],
                //						   vertices[v2 - 1],
                //						   vertices[v3 - 1],
                //						   velocityVertices[v1 - 1],
                //						   velocityVertices[v2 - 1],
                //						   velocityVertices[v3 - 1],
                //						   (normals[n1] + normals[n2] + normals[n3]) / 3,
                //						   this);
                //elems.push_back(t);
                addTriangle(v1 - 1, v2 - 1, v3 -1);
            }
            else if (sscanf(buf, "%d/%d/%d", &v1, &t1, &n1) == 3)
            {
                /* v/t/n */

                fscanf(file, "%d/%d/%d", &v2, &t2, &n2);
                fscanf(file, "%d/%d/%d", &v3, &t3, &n3);
                /* Triangle *t = new Triangle(vertices[v1 - 1],
                						   vertices[v2 - 1],
                						   vertices[v3 - 1],
                						   velocityVertices[v1 - 1],
                						   velocityVertices[v2 - 1],
                						   velocityVertices[v3 - 1],
                						   (normals[n1] + normals[n2] + normals[n3]) / 3,
                						   this);
                elems.push_back(t); */
                addTriangle(v1 - 1, v2 - 1, v3 -1);
            }
            else if (sscanf(buf, "%d/%d", &v1, &t1) == 2)
            {
                /* v/t */
                fscanf(file, "%d/%d", &v2, &t2);
                fscanf(file, "%d/%d", &v3, &t3);
                /* Triangle *t = new Triangle(vertices[v1 - 1],
                						   vertices[v2 - 1],
                						   vertices[v3 - 1],
                						   velocityVertices[v1 - 1],
                						   velocityVertices[v2 - 1],
                						   velocityVertices[v3 - 1],
                						   (normals[n1] + normals[n2] + normals[n3]) / 3,
                						   this);
                elems.push_back(t); */
                addTriangle(v1 - 1, v2 - 1, v3 -1);
            }
            else
            {
                /* v */
                sscanf(buf, "%d", &v1);
                fscanf(file, "%d", &v2);
                fscanf(file, "%d", &v3);

                // compute the normal
                /* Triangle *t = new Triangle(vertices[v1 - 1],
                						   vertices[v2 - 1],
                						   vertices[v3 - 1],
                						   velocityVertices[v1 - 1],
                						   velocityVertices[v2 - 1],
                						   velocityVertices[v3 - 1],
                						   this);
                elems.push_back(t); */
                addTriangle(v1 - 1, v2 - 1, v3 -1);
            }
            break;

        default:
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        }
    }

    if (normals.empty())
    {
        // compute the normal for the triangles
        /*		std::vector<CollisionElement*>::iterator it = elems.begin()
        		std::vector<CollisionElement*>::iterator itEnd = elems.end();

        		for (; it != itEnd; it++)
        		{
        			Triangle *t = static_cast<Triangle*> (*it);
        			Vector3 u,v;
        			u = *(t->p2) - *(t->p1);
        			v = *(t->p3) - *(t->p1);

        			Vector3 uCrossV = u.Cross(v);

        			t->normal = uCrossV;
        		}*/
    }
}

} // namespace io

} // namespace helper

} // namespace sofa

