use wgpu::{
    BufferAddress, BufferDescriptor, BufferUsages
};
use std::{
    sync::Arc,
    num::NonZero
};
use crate::{
    cube::cube, vertex::Vertex, camera::Camera, camera::CameraUniform,
    vertex::RenderInstanceData,
};
use wgpu::{util::DeviceExt};

/// Maximum number of instances of a rendered object to be rendered at a time.
const MAX_INSTANCES: usize = 16384;

/// A single item of a rendering job.
pub struct RenderTask {
    vertex_buf: wgpu::Buffer,
    index_buf: wgpu::Buffer,
    index_count: usize,
    bind_group: wgpu::BindGroup,
    uniform_buf: wgpu::Buffer,
    instance_buf: wgpu::Buffer,
    instances: usize,
    instance_data: [RenderInstanceData; MAX_INSTANCES],
    pipeline: wgpu::RenderPipeline,
}

impl RenderTask {

    pub fn new(
        device: Arc<wgpu::Device>,
        queue: Arc<wgpu::Queue>,
        surface_config: wgpu::SurfaceConfiguration,
        instances: usize
    ) -> Self {
        // generate vertices for the mesh
        let (vertex_data, index_data) = Self::create_vertices_from_cube();

        // create buffers from the vertex and index data
        let (vertex_buf, index_buf) = Self::create_buffers_from_vertex_and_index_data(
            device.clone(), &vertex_data, &index_data);

        let bind_group_layout = Self::get_bind_group_layout(&device);

        // Create the texture
        let size = 256u32;
        let texels = Self::create_texels_from_mandelbrot(size as usize);
        let texture_extent = wgpu::Extent3d {
            width: size,
            height: size,
            depth_or_array_layers: 1,
        };
        let texture = device.create_texture(&wgpu::TextureDescriptor {
            label: None,
            size: texture_extent,
            mip_level_count: 1,
            sample_count: 1,
            dimension: wgpu::TextureDimension::D2,
            format: wgpu::TextureFormat::R8Uint,
            usage: wgpu::TextureUsages::TEXTURE_BINDING | wgpu::TextureUsages::COPY_DST,
            view_formats: &[],
        });
        let texture_view = texture.create_view(&wgpu::TextureViewDescriptor::default());
        queue.write_texture(
            texture.as_image_copy(),
            &texels,
            wgpu::TexelCopyBufferLayout {
                offset: 0,
                bytes_per_row: Some(size),
                rows_per_image: None,
            },
            texture_extent,
        );

        // Create the uniform buffer to hold the camera matrices
        let uniform_buf = device.create_buffer(&BufferDescriptor {
            label: Some("Camera Uniform Buffer"),
            size: core::mem::size_of::<CameraUniform>() as u64,
            usage: BufferUsages::UNIFORM | BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        // Create the buffer for holding the per-instance data
        let instance_buf = device.create_buffer(&BufferDescriptor {
            label: Some("Instance Buffer"),
            size: (core::mem::size_of::<RenderInstanceData>() * MAX_INSTANCES) as u64,
            usage: BufferUsages::VERTEX | BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        // Create bind group
        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: uniform_buf.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::TextureView(&texture_view),
                },
            ],
            label: None,
        });

        let shader = device.create_shader_module(wgpu::include_wgsl!("shader.wgsl"));

        let pipeline = Self::create_render_pipeline(device.clone(), surface_config.clone(), &shader);

        // Done
        RenderTask {
            vertex_buf,
            index_buf,
            index_count: index_data.len(),
            bind_group,
            uniform_buf,
            instance_buf,
            instances,
            instance_data: [RenderInstanceData::default(); MAX_INSTANCES],
            pipeline,
        }
    }

    fn create_vertices_from_cube() -> (Vec<Vertex>, Vec<u16>) {
        cube()
    }

    pub fn create_buffers_from_vertex_and_index_data(
        device: Arc<wgpu::Device>,
        vertices: &[Vertex],
        indices: &[u16])
        -> (wgpu::Buffer, wgpu::Buffer)
    {
        let vertex_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Vertex Buffer"),
            contents: bytemuck::cast_slice(&vertices),
            usage: wgpu::BufferUsages::VERTEX,
        });

        let index_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Index Buffer"),
            contents: bytemuck::cast_slice(&indices),
            usage: wgpu::BufferUsages::INDEX,
        });

        (vertex_buffer, index_buffer)
    }

    /// Creates a texel map using the Mandelbrot fractal.
    fn create_texels_from_mandelbrot(size: usize) -> Vec<u8> {
        (0..size * size)
            .map(|id| {
                // get high five for recognizing this ;)
                let cx = 3.0 * (id % size) as f32 / (size - 1) as f32 - 2.0;
                let cy = 2.0 * (id / size) as f32 / (size - 1) as f32 - 1.0;
                let (mut x, mut y, mut count) = (cx, cy, 0);
                while count < 0xFF && x * x + y * y < 4.0 {
                    let old_x = x;
                    x = x * x - y * y + cx;
                    y = 2.0 * old_x * y + cy;
                    count += 1;
                }
                count
            })
            .collect()
    }

    pub const VERTEX_BUFFER_LAYOUT: [wgpu::VertexBufferLayout<'_>; 2] = [
        wgpu::VertexBufferLayout {
            array_stride: size_of::<Vertex>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Vertex,
            attributes: &Vertex::ATTRIBUTES,
        },
        wgpu::VertexBufferLayout {
            array_stride: size_of::<RenderInstanceData>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Instance,
            attributes: &RenderInstanceData::ATTRIBUTES
        }
    ];

    /// Returns the bind group layout to use with this render task.
    fn get_bind_group_layout(device: &wgpu::Device) -> wgpu::BindGroupLayout {
        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            label: None,
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: wgpu::BufferSize::new(core::mem::size_of::<CameraUniform>() as u64),
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 1,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        multisampled: false,
                        sample_type: wgpu::TextureSampleType::Uint,
                        view_dimension: wgpu::TextureViewDimension::D2,
                    },
                    count: None,
                },
            ],
        });
        bind_group_layout
    }

    /// Returns the pipeline layout suitable for this task.
    fn get_pipeline_layout(device: &wgpu::Device) -> wgpu::PipelineLayout {
        let bind_group_layout = Self::get_bind_group_layout(device);
        let pipeline_layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
            label: None,
            bind_group_layouts: &[Some(&bind_group_layout)],
            immediate_size: 0,
        });
        pipeline_layout
    }

    /// Creates a rendering pipeline suitable for this task.
    fn create_render_pipeline(
            device: Arc<wgpu::Device>,
            surface_config: wgpu::SurfaceConfiguration,
            shader: &wgpu::ShaderModule
        ) -> wgpu::RenderPipeline
    {
        let pipeline_layout = Self::get_pipeline_layout(&device);

        device.create_render_pipeline(
            &wgpu::RenderPipelineDescriptor {
                label: None,
                layout: Some(&pipeline_layout),
                vertex: wgpu::VertexState {
                    module: &shader,
                    entry_point: Some("vs_main"),
                    compilation_options: Default::default(),
                    buffers: &Self::VERTEX_BUFFER_LAYOUT,
                },
                fragment: Some(wgpu::FragmentState {
                    module: &shader,
                    entry_point: Some("fs_main"),
                    compilation_options: Default::default(),
                    targets: &[Some(surface_config.view_formats[0].into())],
                }),
                primitive: wgpu::PrimitiveState {
                    cull_mode: Some(wgpu::Face::Back),
                    ..Default::default()
                },
                depth_stencil: None,
                multisample: wgpu::MultisampleState::default(),
                multiview_mask: None,
                cache: None,
            }
        )
    }

    /// Submits a GPU work that clears the screen/frame.
    fn submit_frame_clearing_work(&self, view: &wgpu::TextureView, device: &wgpu::Device, queue: &wgpu::Queue) {
        let mut encoder = device.create_command_encoder(&Default::default());

        // Create the renderpass which will clear the screen.
        let renderpass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: None,
            color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                view: &view,
                depth_slice: None,
                resolve_target: None,
                ops: wgpu::Operations {
                    load: wgpu::LoadOp::Clear(wgpu::Color::BLUE),
                    store: wgpu::StoreOp::Store,
                },
            })],
            depth_stencil_attachment: None,
            timestamp_writes: None,
            occlusion_query_set: None,
            multiview_mask: None,
        });

        // End the renderpass.
        drop(renderpass);

        // finish encoding, obtaining a CommandBuffer
        let cmds = encoder.finish();

        // submit the work
        queue.submit([ cmds ]);
    }

    /// Submits a GPU work that sets up the camera uniforms to reflect the current state of the camera.
    fn submit_camera_uniform_setup_work(&self,
        _view: &wgpu::TextureView,
        _device: &wgpu::Device,
        queue: &wgpu::Queue,
        camera: &Camera)
    {
        // recalculate camera uniforms according to the camera state
        let camera_uniform = camera.as_uniform();

        // send new values of camera uniforms
        queue.write_buffer_with(
            &self.uniform_buf,
            0 as BufferAddress,
            NonZero::new(size_of::<CameraUniform>() as u64).unwrap()
        )
            .expect("camera uniform buffer should be writable")
            .copy_from_slice(bytemuck::bytes_of(&camera_uniform));
        queue.submit([]);
    }

    fn submit_instance_setup_work(&self,
        _view: &wgpu::TextureView,
        _device: &wgpu::Device,
        queue: &wgpu::Queue,
        _camera: &Camera)
    {
        // upload new positions of instances into GPU
        queue.write_buffer_with(
            &self.instance_buf,
            0 as BufferAddress,
            NonZero::new((size_of::<RenderInstanceData>() * MAX_INSTANCES) as u64).unwrap()
        )
            .expect("instance data buffer should be writable")
            .copy_from_slice(bytemuck::bytes_of(&self.instance_data));    // SUS! check!
        queue.submit([]);
    }

    pub fn render(&mut self,
        view: &wgpu::TextureView,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        camera: &Camera)
    {
        self.submit_frame_clearing_work(view, device, queue);
        self.submit_camera_uniform_setup_work(view, device, queue, camera);
        self.submit_instance_setup_work(view, device, queue, camera);

        let mut encoder = device.create_command_encoder(&wgpu::CommandEncoderDescriptor { label: None });

        let mut rpass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: None,
            color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                view,
                depth_slice: None,
                resolve_target: None,
                ops: wgpu::Operations {
                    load: wgpu::LoadOp::Load,
                    store: wgpu::StoreOp::Store,
                },
            })],
            depth_stencil_attachment: None,
            timestamp_writes: None,
            occlusion_query_set: None,
            multiview_mask: None,
        });

        rpass.push_debug_group("Prepare data for draw.");
        rpass.set_pipeline(&self.pipeline);
        rpass.set_bind_group(0, &self.bind_group, &[]);
        rpass.set_index_buffer(self.index_buf.slice(..), wgpu::IndexFormat::Uint16);
        rpass.set_vertex_buffer(0, self.vertex_buf.slice(..));
        rpass.set_vertex_buffer(1, self.instance_buf.slice(..));
        rpass.pop_debug_group();

        rpass.insert_debug_marker("Draw!");
        rpass.draw_indexed(0..self.index_count as u32, 0, 0..(self.instances as u32));

        // indicate that the render pass is complete
        drop(rpass);

        // encode it into a command buffer
        let cmds = encoder.finish();

        // submit it for rendering
        queue.submit([ cmds ]);
    }

    pub fn set_active_instances(&mut self, count: u32) {
        self.instances = count as usize;
    }

    pub fn set_instance_position(&mut self, instance_number: u32, pos: [f32; 4]) {
        self.instance_data[instance_number as usize].pos = pos;
    }
}
