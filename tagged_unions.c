#include <stdio.h>

union {
    enum {

    }
}
enum {
    Tag1(char)
}


/*
enum UnionTag {
    Type1 = 0,
};

struct Type1{
    char q;
};

struct taggedUnion_{
    union{
        struct Type1 t1;
    };
    enum UnionTag tag;
};

struct taggedUnion{
    union{
        struct Type1 t1;
    };
};

struct taggedUnion_ f;

struct taggedUnion *g = (struct taggedUnion*)&f;

typedef enum Tag {tag1, tag2, tag3} Tag;

typedef union Union {
  int value1;
  char value2;
  void *value3;
} Union;

typedef struct TaggedUnion {
  Tag tag;
  Union value;
} TaggedUnion;

*/
