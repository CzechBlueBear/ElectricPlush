
use std::f32::consts;

#[allow(unused)]
#[derive(Default)]
pub struct Camera {
	pos: glam::Vec3,
	zoom: f32,
	aspect: f32,
	min_depth: f32,
	max_depth: f32,
}

impl Camera {
	pub fn new() -> Camera {
		Camera {
			pos: glam::Vec3::new(1.5f32, -5.0, 3.0),
			zoom: 1.0,
			aspect: 1.0,
			min_depth: 1.0,
			max_depth: 10.0,
		}
	}

	/// Builds camera matrices and returns them as a structure that is
	/// compatible with GPU uniforms.
	pub fn as_uniform(&self) -> CameraUniform {
		CameraUniform {
			projection_matrix: self.projection_matrix().to_cols_array(),
			view_matrix: self.view_matrix().to_cols_array()
		}
	}

	/// Generates a projection matrix reflecting the current camera state.
	pub fn projection_matrix(&self) -> glam::Mat4 {
		glam::Mat4::perspective_rh(
			consts::FRAC_PI_4,
			self.aspect,
			self.min_depth,
			self.max_depth
		)
	}

	/// Generates a view matrix reflecting the current camera position and viewpoint.
	pub fn view_matrix(&self) -> glam::Mat4 {
        glam::Mat4::look_at_rh(
            self.pos,
            glam::Vec3::ZERO,
            glam::Vec3::Z,
        )
	}
}

/// Structure for holding the camera matrices for use by shaders;
/// this needs to be in a precise form that is understood by wgsl.
/// Matrices are stored in left-to-right, column-major order.
#[repr(C)]
#[derive(Default, Debug, Copy, Clone, bytemuck::Pod, bytemuck::Zeroable)]
pub struct CameraUniform {
    projection_matrix: [f32; 16],
    view_matrix: [f32; 16]
}
