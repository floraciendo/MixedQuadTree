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
* @file QuadEdge.cpp
* @author Claudio Lobos, Fabrice Jaillet
* @version 0.1
* @brief
**/

#include "QuadEdge.h"

namespace Clobscode
{
	
// vector<unsigned int> info; //info[2] midpoint

	QuadEdge::QuadEdge(){
		info.assign(2,0);
	}
	
    QuadEdge::QuadEdge(const unsigned int &point1, const unsigned int &point2){
        info.resize(2);
        assign(point1, point2);
    }
    
    
    //use this constructor when sure point1<point2 
    QuadEdge::QuadEdge(const unsigned int &point1, const unsigned int &point2,
                       const bool &forced){
        info.resize(2);
        info[0]=point1;
        info[1]=point2;
    }

	QuadEdge::~QuadEdge(){
		
	}
    
    void QuadEdge::assign(unsigned int point1, unsigned int point2){
        if (point1<point2) {
			info[0]=point1;
			info[1]=point2;
		}
		else {
			info[1]=point1;
			info[0]=point2;
		}
	}
	
	ostream& operator<<(ostream& o, const QuadEdge &e){
		o << e[0] << " ";
        o << e[1];
		return o;
	}
	
    bool operator==(const QuadEdge &e1, const QuadEdge &e2) {
		//this possible as QuadEdges are sorted by min index
		if (e1[0]==e2[0] && e1[1]==e2[1])
			return true;
		return false;
	}
	
    bool operator!=(const QuadEdge &e1, const QuadEdge &e2) {
		return !(e1==e2);
	}

    bool operator<(const QuadEdge &e1, const QuadEdge &e2) {
		if (e1[0]!=e2[0]){
			return e1[0]<e2[0];
		}
		return e1[1] < e2[1];
	}
	
}
