#include "3Dgba/rasterize.h"

namespace gba_3D {



constexpr int MAX_DEPTH = 100;
IWRAM_DATA static std::array<uint16_t, MAX_DEPTH> triangle_base_indicies = {0}; // holds the index of the next triangle at depth

EWRAM_DATA ALIGN(16) std::array<transformed_face, 1000> transformed_faces;
uint16_t face_count = 0;

void sort_inplace(vertex_int16_u (&a)[3]){
    int16_t swap = a[0].i;
    if (a[0].i > a[1].i){
        a[0].i = a[1].i;
        a[1].i = swap;
    }
    if (a[0].i > a[2].i){
        swap = a[0].i;
        a[0].i = a[2].i;
        a[2].i = swap;
    }
    if (a[1].i > a[2].i){
        swap = a[1].i;
        a[1].i = a[2].i;
        a[2].i = swap;
    }
}

void add_transformed_face(const Triangle2Ds_t& triangle, const uint16_t depth, const uint8_t color){
    transformed_faces[face_count].color = color;
    transformed_faces[face_count].triangle = triangle;
    transformed_faces[face_count].next_index = triangle_base_indicies[depth];
    triangle_base_indicies[depth] = face_count;
    face_count++;
}

void fillBottomFlatTriangle(Vector2c_t (&tri)[3], void* fb)
{
  float invslope1 = (tri[1].x - tri[0].x) / (tri[1].y - tri[0].y);
  float invslope2 = (tri[2].x - tri[0].x) / (tri[2].y - tri[0].y);

  float curx1 = tri[0].x;
  float curx2 = tri[0].x;

  for (int scanlineY = tri[0].y; scanlineY <= tri[1].y; scanlineY++)
  {
    for (int i = curx1; i <= curx2; i++){
        ((uint8_t*)(fb))[scanlineY * 240 + (int)i] = 0x14;
    }
    curx1 += invslope1;
    curx2 += invslope2;
  }
}

void fillTopFlatTriangle(Vector2c_t (&tri)[3], void* fb)
{
  float invslope1 = (tri[2].x - tri[0].x) / (tri[2].y - tri[0].y);
  float invslope2 = (tri[2].x - tri[1].x) / (tri[2].y - tri[1].y);

  float curx1 = tri[2].x;
  float curx2 = tri[2].x;

  for (int scanlineY = tri[2].y; scanlineY > tri[0].y; scanlineY--)
  {
    for (int i = curx1; i <= curx2; i++){
        ((uint8_t*)(fb))[scanlineY * 240 + (int)i] = 0x14;
    }
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

void drawTriangle(Vector2c_t (&tri)[3], void* fb)
{
   /* at first sort the three vertices by y-coordinate ascending so tri[0] is the topmost vertice */
    sort_inplace(*reinterpret_cast<vertex_int16_u(*)[3]>(&tri));

  /* here we know that tri[0].y <= tri[1].y <= tri[2].y */
  /* check for trivial case of bottom-flat triangle */
  if (tri[1].y == tri[2].y)
  {
    fillBottomFlatTriangle(tri, fb);
  }
  /* check for trivial case of top-flat triangle */
  else if (tri[0].y == tri[1].y)
  {
    fillTopFlatTriangle(tri, fb);
  }
  else
  {
    /* general case - split the triangle in a topflat and bottom-flat one */
    Vector2c_t v4 = {
      (int)(tri[0].x + ((float)(tri[1].y - tri[0].y) / (float)(tri[2].y - tri[0].y)) * (tri[2].x - tri[0].x)), tri[1].y};

    Vector2c_t tri2[3] = {tri[0], tri[1], v4};
    fillBottomFlatTriangle(tri2, fb);
    Vector2c_t tri3[3] = {tri[1], v4, tri[2]};
    fillTopFlatTriangle(tri3, fb);
  }
}

IWRAM_CODE ARM_CODE void rasterize(void* fb){
    for (int i = 0; i < MAX_DEPTH; i++){
        if(triangle_base_indicies[i] == 0) continue;

        uint16_t index = triangle_base_indicies[i];
        while(index != 0){
            const auto& face = transformed_faces[index];
            const auto& triangle = face.triangle;
            const auto& color = face.color;
            


            index = face.next_index;
        }
    }
}

}

