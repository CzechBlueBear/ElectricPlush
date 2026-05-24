use core::mem::offset_of;
use bytemuck::{Pod, Zeroable};

/// A single vertex as stored in memory for the GPU to use.
#[repr(C)]
#[derive(Clone, Copy, Pod, Zeroable)]
pub struct Vertex {
    pub pos: [f32; 4],
    pub tex_coord: [f32; 2],
    pub normal: [f32; 4]
}

impl Vertex {

    /// Returns an array of VertexAttribute structures describing the type
    /// and order of attributes in a Vertex, and which shader slot they belong to.
    pub fn attributes() -> &'static [wgpu::VertexAttribute; 3] {
        &[
            wgpu::VertexAttribute {
                format: wgpu::VertexFormat::Float32x4,
                offset: offset_of!(Vertex, pos) as u64,            // offset=0, this is the first item
                shader_location: 0,
            },
            wgpu::VertexAttribute {
                format: wgpu::VertexFormat::Float32x2,
                offset: offset_of!(Vertex, tex_coord) as u64,       // offset = 4*size_of(f32) = 4*4 = 16
                shader_location: 1,
            },
            wgpu::VertexAttribute {
                format: wgpu::VertexFormat::Float32x4,
                offset: offset_of!(Vertex, normal) as u64,         // offset = (4 + 2)*size_of(f32) = 6*4 = 24
                shader_location: 2,
            }
        ]
    }
}

/// Generates a Vertex structure initialized to the given values
/// (specified as integers but are stored as floats).
pub fn vertex_i(pos: [i8; 3], tc: [i8; 2], normal: [i8; 3]) -> Vertex {
    Vertex {
        pos: [pos[0] as f32, pos[1] as f32, pos[2] as f32, 1.0],
        tex_coord: [tc[0] as f32, tc[1] as f32],
        normal: [normal[0] as f32, normal[1] as f32, normal[2] as f32, 0.0]
    }
}

/// Information of where to put an instance of a rendered object.
#[repr(C)]
#[derive(Clone, Copy, Pod, Zeroable)]
pub struct RenderInstanceData {
    pub pos: [f32; 4]
}
