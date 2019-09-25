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
* @file SplitVisitor.cpp
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#include "SplitVisitor.h"
#include "../Quadrant.h"

namespace Clobscode
{
//vector<MeshPoint> *points;
//list<Point3D> *new_pts;
//set<QuadEdge> *edges;
//vector<vector<unsigned int> > *new_eles;
//vector<vector<Point3D> > *clipping;

    SplitVisitor::SplitVisitor()
        :points(NULL),new_pts(NULL),mapedges(NULL),new_eles(NULL),clipping(NULL)
    { }

    void SplitVisitor::setPoints(const vector<MeshPoint> &points) {
        this->points = &points;
    }
    
    void SplitVisitor::setNewPts(list<Point3D> &new_pts) {
        this->new_pts = &new_pts;
    }
    
    void SplitVisitor::setMapEdges(map<QuadEdge, unsigned int> &mapedges) {
        this->mapedges = &mapedges;
    }
    
    void SplitVisitor::setNewEles(vector<vector<unsigned int> > &new_eles) {
        this->new_eles = &new_eles;
    }
    
    void SplitVisitor::setClipping(vector<vector<Point3D> > &clipping) {
        this->clipping = &clipping;
    }

    bool SplitVisitor::visit(Quadrant *o)
    {
        //getting variables for modification
        //preferably by reference, to avoid unnecessary copying
        const vector<unsigned int> &pi = o->pointindex;

        new_eles->reserve(4);

        unsigned int n_pts = points->size() + new_pts->size();
        //the vector containing all nodes of this Quadrant (and sons)
        vector<unsigned int> all_pts(9,0);

        //save the four nodes of this square first
        for (unsigned int i=0; i< pi.size(); i++) {
            all_pts[i] = pi[i];
        }

        const Point3D &min = points->at(pi[0]).getPoint();
        const Point3D &max = points->at(pi[2]).getPoint();
        const Point3D avg = (max-min)/2 + min;

        //inserting node 4 between nodes 0 and 1
        if (splitEdge(all_pts[0],all_pts[1],n_pts,all_pts[4])) {
            //the coordinates of node 8 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],min[1],avg[2]));
        }
        //inserting node 5 between nodes 1 and 2
        if (splitEdge(all_pts[1],all_pts[2],n_pts,all_pts[5])) {
            //the coordinates of node 9 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (max[0],avg[1],avg[2]));
        }
        //inserting node 6 between nodes 2 and 3
        if (splitEdge(all_pts[2],all_pts[3],n_pts,all_pts[6])) {
            //the coordinates of node 10 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (avg[0],max[1],avg[2]));
        }
        //inserting node 7 between nodes 3 and 0
        if (splitEdge(all_pts[0],all_pts[3],n_pts,all_pts[7])) {
            //the coordinates of node 11 must be computed and added to
            //new_pts list of points
            new_pts->push_back(Point3D (min[0],avg[1],avg[2]));
        }

        //of course all the intern edges and mid point were never inserted
        //before, so this task is performed without asking
        new_pts->push_back(Point3D (avg[0],avg[1],avg[2]));
        all_pts[8] = n_pts;

        //NOTE: edge (4,6) and (5,7) with mid_point 8 (the center of the
        //Quad are not inserted because those edges ((4,6) and (5,7)) don't
        //belong to any element. These edges would only increase the use of
        //memory without adding necessary information.
        //edges->emplace(all_pts[4],all_pts[6],all_pts[8]);
        //edges->emplace(all_pts[5],all_pts[7],all_pts[8]);
        QuadEdge qeY(all_pts[4],all_pts[6]);
        mapedges->emplace(qeY,all_pts[8]);
        
        QuadEdge qeX(all_pts[5],all_pts[7]);
        mapedges->emplace(qeX,all_pts[8]);
        
        QuadEdge qe1(all_pts[4],all_pts[8]);
        mapedges->emplace(qe1,0);
        
        QuadEdge qe2(all_pts[6],all_pts[8]);
        mapedges->emplace(qe2,0);
        
        QuadEdge qe3(all_pts[5],all_pts[8]);
        mapedges->emplace(qe3,0);
        
        QuadEdge qe4(all_pts[7],all_pts[8]);
        mapedges->emplace(qe4,0);
        
        //now that all edges were inserted, the elements can be easily built
        vector<unsigned int> son_element (4,0);
        son_element[0]=all_pts[0];
        son_element[1]=all_pts[4];
        son_element[2]=all_pts[8];
        son_element[3]=all_pts[7];

        new_eles->push_back(son_element);

        son_element[0]=all_pts[4];
        son_element[1]=all_pts[1];
        son_element[2]=all_pts[5];
        son_element[3]=all_pts[8];

        new_eles->push_back(son_element);

        son_element[0]=all_pts[8];
        son_element[1]=all_pts[5];
        son_element[2]=all_pts[2];
        son_element[3]=all_pts[6];

        new_eles->push_back(son_element);

        son_element[0]=all_pts[7];
        son_element[1]=all_pts[8];
        son_element[2]=all_pts[6];
        son_element[3]=all_pts[3];

        new_eles->push_back(son_element);

        //extreme nodes of each son to be used by clipping
        //method.
        clipping->reserve(4);
        vector<Point3D> extreme_nodes(2, Point3D ());

        //bottom/left son is defined by nodes 0 and 8
        extreme_nodes[0] = Point3D (min.X(),min.Y(),avg.Z());
        extreme_nodes[1] = Point3D (avg.X(),avg.Y(),avg.Z());
        clipping->push_back(extreme_nodes);

        //bottom/right son is defined by nodes 4 and 5
        extreme_nodes[0] = Point3D (avg.X(),min.Y(),avg.Z());
        extreme_nodes[1] = Point3D (max.X(),avg.Y(),avg.Z());
        clipping->push_back(extreme_nodes);

        //top/right son is defined by nodes 8 and 2
        extreme_nodes[0] = Point3D (avg.X(),avg.Y(),avg.Z());
        extreme_nodes[1] = Point3D (max.X(),max.Y(),avg.Z());
        clipping->push_back(extreme_nodes);

        //bottom/left son is defined by nodes 7 and 6
        extreme_nodes[0] = Point3D (min.X(),avg.Y(),avg.Z());
        extreme_nodes[1] = Point3D (avg.X(),max.Y(),avg.Z());
        clipping->push_back(extreme_nodes);

        return true;
    }

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


    bool SplitVisitor::splitEdge(unsigned int idx1, unsigned int idx2,
                                 unsigned int &c_n_pts, unsigned int &mid_idx){
        
        QuadEdge this_edge (idx1,idx2);

        mid_idx = (*mapedges)[this_edge];
        if (mid_idx!=0) {
            return false;
        }
        
        //this edge is about to be split. Note that no edge can have point index
        //0 as its mid_point. For this reason, we know that the edge was not
        //split by other Quadrant before. The current edge must be replaced in the
        //set by the same one plus a mid_point equal to c_n_pts (current number
        //of points). The coordinates of this new point will be inserted by the
        //split method above. The splitEdge method will only compute the index
        //for this new point and will insert the two new edges (derived from the
        //splitting process of the current edge). Note that c_n_pts will be
        //increased for next splitting process of another edge.
        
        mid_idx = c_n_pts;
        QuadEdge qe1(idx1,c_n_pts), qe2(idx2,c_n_pts);
        mapedges->emplace(qe1,0);
        mapedges->emplace(qe2,0);
        (*mapedges)[this_edge] = c_n_pts++;
        
        return true;
    }

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

    /*bool SplitVisitor::splitFace(unsigned int idx1, unsigned int idx2,
                                 unsigned int idx3, unsigned int idx4,
                                 unsigned int &c_n_pts, unsigned int &mid_idx){
        QuadEdge e1 (idx1,idx3);
        set<QuadEdge>::iterator found = edges->find(e1);

        if (found==edges->end()) {
            //this face wasn't split before->
            e1.updateMidPoint(c_n_pts);
            edges->insert(e1);

            QuadEdge e2 (idx2, idx4);
            e2.updateMidPoint(c_n_pts);
            edges->insert(e2);

            //splitting edge e1
            edges->insert(QuadEdge (idx1,c_n_pts));
            edges->insert(QuadEdge (idx3,c_n_pts));
            //splitting edge e2
            edges->insert(QuadEdge (idx2,c_n_pts));
            edges->insert(QuadEdge (idx4,c_n_pts));

            //increase the number fo total points
            mid_idx = c_n_pts++;

            return true;
        }

        //at this point, the face was already split. Update the mid index
        mid_idx = (*found)[2];
        return false;
    }*/

}


