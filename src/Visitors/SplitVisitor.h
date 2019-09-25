/*
 <Mix-mesher: region type. This program generates a mixed-elements 2D mesh>

 Copyright (C) <2013,2018>  <Claudio Lobos> All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
 */
/**
* @file SplitVisitor.h
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#ifndef SplitVisitor_h
#define SplitVisitor_h 1

#include <list>
#include <set>
#include <vector>
#include <map>

#include "../MeshPoint.h"
#include "../Point3D.h"
#include "../QuadEdge.h"

#include "Visitor.h"


using Clobscode::MeshPoint;
using Clobscode::QuadEdge;
using Clobscode::Point3D;
using std::vector;
using std::list;
using std::map;
using std::set;


namespace Clobscode
{
    class SplitVisitor : public Visitor {

    public:
        
        SplitVisitor();

        bool visit(Quadrant *o) override;

        void setPoints(const vector<MeshPoint> &points);
        
        void setNewPts(list<Point3D> &new_pts);
        
        void setEdges(set<QuadEdge> &edges);
        
        void setMapEdges(map<QuadEdge, unsigned int> &mapedges);
        
        void setNewEles(vector<vector<unsigned int> > &new_eles);
        
        void setClipping(vector<vector<Point3D> > &clipping);

    protected:
        
        //references
        const vector<MeshPoint> *points;
        list<Point3D> *new_pts;
        set<QuadEdge> *edges;
        map<QuadEdge, unsigned int> *mapedges;
        
        vector<vector<unsigned int> > *new_eles;
        vector<vector<Point3D> > *clipping;

        bool splitEdge(unsigned int idx1,
                       unsigned int idx2,
                       unsigned int &c_n_pts,
                       unsigned int &mid_idx);

    };
}



#endif //MESHER_ROI_SPLITVISITOR_H
