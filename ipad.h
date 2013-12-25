#ifndef IPAD_H_DEFINED
#define IPAD_H_DEFINED

#define LEVEL0 0
#define LEVEL1 1
#define LEVEL2 2
#define DECISION 50
#define STORAGE 100
#define HANDBOOK 150
#define HANDBOOK_GREEN 151
#define HANDBOOK_YELLOW 152
#define HANDBOOK_BLUE 153
#define GRAYBOX 200
#define YELLOWBOX 201
#define GROUPBOX 202

#define LABEL 500

#define LINEPOINT 2000

#define LINE 5000
#define DASH_LINE 5001
#define DOT_LINE 5002

#define ARROW_UP 3000
#define ARROW_RIGHT 3001
#define ARROW_DOWN 3002
#define ARROW_LEFT 3003

#define HEADER_H_OFFSET 0
#define HEADER_W_OFFSET 180

#ifndef RETINA
#define IPAD_WIDTH (1024)
#define IPAD_HEIGHT (728)
#else
#define IPAD_WIDTH (2048-HEADER_W_OFFSET)
#define IPAD_HEIGHT (1456-HEADER_H_OFFSET)
#endif //RETINA

#endif //IPAD_H_DEFINED