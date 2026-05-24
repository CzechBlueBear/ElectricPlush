use crate::{
    vertex::Vertex, vertex::vertex_i
};

// A unit cube.
// Assuming a right-handed, Z-up coordinate system, i.e.:
//
//    * +X points leftwards
//    * +Y points upwards
//    * +Z points towards the front plane (out of the screen)
//
// In this arrangement, the front side of an axis-aligned cube
// has a normal vector of (0, 0, +1)
//
// Corner points on the cube are formally named A, B, C, D (bottom side)
// and E, F, G, H (top side).
//
// Texture coordinates are assumed to have [0, 0] as the top left point,
// X pointing leftwards, and Y pointing down.
//
// Points on each side are ordered so that they appear in a positive
// (counterclockwise) order for a viewer looking directly at the cube's side
// from the outside.
//
// Texture coordinates for the cube are generated so that, for a viewer looking at
// the given side, the [0, 0] point of the texture (the topleft point)
// appears to be in the top left corner of the cube's side.

pub fn cube() -> (Vec<Vertex>, Vec<u16>) {
    let vertex_data = [

        // front side (ABEF)
        vertex_i([ -1, -1,  1], [0, 1], [0, 0, 1]),     // #0
        vertex_i([  1, -1,  1], [1, 1], [0, 0, 1]),     // #1
        vertex_i([  1,  1,  1], [1, 0], [0, 0, 1]),     // #2
        vertex_i([ -1,  1,  1], [0, 0], [0, 0, 1]),     // #3

        // top side (EFGH)
        vertex_i([ -1,  1,  1], [0, 1], [0, 1, 0]),     // #4
        vertex_i([  1,  1,  1], [1, 1], [0, 1, 0]),     // #5
        vertex_i([  1,  1, -1], [1, 0], [0, 1, 0]),     // #6
        vertex_i([ -1,  1, -1], [0, 0], [0, 1, 0]),     // #7

        // bottom side (ADCB)
        vertex_i([ -1, -1,  1], [0, 0], [0, -1, 0]),    // #8
        vertex_i([ -1, -1, -1], [0, 1], [0, -1, 0]),    // #9
        vertex_i([  1, -1, -1], [1, 1], [0, -1, 0]),    // #10
        vertex_i([  1, -1,  1], [1, 0], [0, -1, 0]),    // #11

        // back side (DHGC)
        vertex_i([ -1, -1, -1], [1, 1], [0, 0, -1]),    // #12
        vertex_i([ -1,  1, -1], [1, 0], [0, 0, -1]),    // #13
        vertex_i([  1,  1, -1], [0, 0], [0, 0, -1]),    // #14
        vertex_i([  1, -1, -1], [0, 1], [0, 0, -1]),    // #15

        // left side (AEHD)
        vertex_i([ -1, -1,  1], [1, 1], [-1, 0, 0]),    // #16
        vertex_i([ -1,  1,  1], [1, 0], [-1, 0, 0]),    // #17
        vertex_i([ -1,  1, -1], [0, 0], [-1, 0, 0]),    // #18
        vertex_i([ -1, -1, -1], [0, 1], [-1, 0, 0]),    // #19

        // right side (BCGF)
        vertex_i([ 1, -1,  1],  [0, 1], [1, 0, 0]),     // #20
        vertex_i([ 1, -1, -1],  [1, 1], [1, 0, 0]),     // #21
        vertex_i([ 1,  1, -1],  [1, 0], [1, 0, 0]),     // #22
        vertex_i([ 1,  1,  1],  [0, 0], [1, 0, 0]),     // #23
    ];

    let index_data: &[u16] = &[
          0, 1, 2,        0, 2, 3,        // front side
          4, 5, 6,        4, 6, 7,        // top side
          8, 9, 10,       8, 10, 11,      // bottom side
        12, 13, 14,      12, 14, 15,      // back side
        16, 17, 18,      16, 18, 19,      // left side
        20, 21, 22,      20, 22, 23,      // right side
    ];

    (vertex_data.to_vec(), index_data.to_vec())
}

pub fn cube_shifted(shift: glam::Vec3) -> (Vec<Vertex>, Vec<u16>) {
    let (vertices, indices) = cube();
    let shifted_vertices = vertices.into_iter().map(|v| {
        Vertex {
            pos: [ v.pos[0] + shift.x, v.pos[1] + shift.y, v.pos[2] + shift.z, 1.0 ],
            normal: v.normal,
            tex_coord: v.tex_coord
        }
    }).collect();
    (shifted_vertices, indices)
}