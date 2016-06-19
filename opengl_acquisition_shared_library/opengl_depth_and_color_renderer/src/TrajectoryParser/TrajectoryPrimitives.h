 /** @file TrajectoryPrimitives.h
* @brief A tool that scans for a string in a very fast and robust way
* @author Ammar Qammaz (AmmarkoV)
*/

#ifndef TRAJECTORYPRIMITIVES_H_INCLUDED
#define TRAJECTORYPRIMITIVES_H_INCLUDED


/** @brief Enumerator for the IDs of TrajectoryPrimitives so we can know what the result was*/
enum { 
 TRAJECTORYPRIMITIVES_EMPTY=0,
 TRAJECTORYPRIMITIVES_COMMENT, // 1 
 TRAJECTORYPRIMITIVES_ARROW, // 2 
 TRAJECTORYPRIMITIVES_POS, // 3 
 TRAJECTORYPRIMITIVES_DEBUG, // 4 
 TRAJECTORYPRIMITIVES_TIMESTAMP, // 5 
 TRAJECTORYPRIMITIVES_AUTOREFRESH, // 6 
 TRAJECTORYPRIMITIVES_INTERPOLATE_TIME, // 7 
 TRAJECTORYPRIMITIVES_ALWAYS_SHOW_LAST_FRAME, // 8 
 TRAJECTORYPRIMITIVES_BACKGROUND, // 9 
 TRAJECTORYPRIMITIVES_COMPOSITE_OBJECT, // 10 
 TRAJECTORYPRIMITIVES_RIGID_OBJECT, // 11 
 TRAJECTORYPRIMITIVES_OBJECT_TYPE, // 12 
 TRAJECTORYPRIMITIVES_OBJECTTYPE, // 13 
 TRAJECTORYPRIMITIVES_OBJECT, // 14 
 TRAJECTORYPRIMITIVES_OBJ, // 15 
 TRAJECTORYPRIMITIVES_EVENT, // 16 
 TRAJECTORYPRIMITIVES_PROJECTION_MATRIX, // 17 
 TRAJECTORYPRIMITIVES_EMULATE_PROJECTION_MATRIX, // 18 
 TRAJECTORYPRIMITIVES_MODELVIEW_MATRIX, // 19 
 TRAJECTORYPRIMITIVES_SCALE_WORLD, // 20 
 TRAJECTORYPRIMITIVES_GENERATE_ANGLE_OBJECTS, // 21 
 TRAJECTORYPRIMITIVES_OFFSET_ROTATIONS, // 22 
 TRAJECTORYPRIMITIVES_MAP_ROTATIONS, // 23 
 TRAJECTORYPRIMITIVES_FRAME_RESET, // 24 
 TRAJECTORYPRIMITIVES_FRAME, // 25 
 TRAJECTORYPRIMITIVES_RATE, // 26 
 TRAJECTORYPRIMITIVES_AFFIX_OBJ_TO_OBJ_FOR_NEXT_FRAMES, // 27 
 TRAJECTORYPRIMITIVES_INCLUDE, // 28 
 TRAJECTORYPRIMITIVES_MOVE_VIEW, // 29 
 TRAJECTORYPRIMITIVES_SMOOTH, // 30 
 TRAJECTORYPRIMITIVES_OBJ_OFFSET, // 31 
 TRAJECTORYPRIMITIVES_CONNECTOR, // 32 
 TRAJECTORYPRIMITIVES_HAND_POINTS, // 33 
 TRAJECTORYPRIMITIVES_PQ, // 34 
 TRAJECTORYPRIMITIVES_P, // 35 
 TRAJECTORYPRIMITIVES_END_OF_ITEMS
};



/** @brief Scan a string for one of the words of the TrajectoryPrimitives word set
* @ingroup stringParsing
* @param Input String , to be scanned
* @param Length of Input String
* @retval See above enumerator*/
 int scanFor_TrajectoryPrimitives(const char * str,unsigned int strLength); 

#endif
