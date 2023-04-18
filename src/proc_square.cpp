//#include <servers/rendering/renderer_scene_render.h>
#include "proc_square.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/rendering_server.hpp>

using namespace godot;

ProcSquare::ProcSquare() {
    nav_mesh = Ref<NavigationMesh>(memnew(NavigationMesh));
    nav_region->set_navigation_mesh(nav_mesh);
}

ProcSquare::~ProcSquare() {
    if (nav_mesh.is_valid()) {
        nav_mesh->unreference();
    }
    NavigationServer3D::get_singleton()->free_rid(nav_region->get_region_rid());

    if (collision_shape->is_visible_in_tree()) {
        memfree(collision_shape);
    }
    if (mesh_instance->is_visible_in_tree()) {
        RenderingServer::get_singleton()->free_rid(mesh_instance->get_instance());
        memfree(mesh_instance);
    }
}

void ProcSquare::_bind_methods() {
}

void ProcSquare::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
        } break;
        case NOTIFICATION_ENTER_WORLD: {
        } break;
        case NOTIFICATION_READY: {
            proc_level = reinterpret_cast<Proc *>(get_parent());
            add_child(collision_shape);
            add_child(nav_region);
            if (proc_level->generate_navigation) {
                Ref<NavigationMesh> n_mesh = nav_mesh;
                n_mesh->set_cell_size(2.0f);
                n_mesh->set_agent_radius(2.0f);
                n_mesh->set_agent_max_slope(75.0f);
                n_mesh->set_agent_max_climb(3.0f);

            }
            nav_region->set_navigation_mesh(nav_mesh);
            nav_region->add_child(mesh_instance);

        } break;
    }
}

void ProcSquare::generate(float square_size, float int_dis, float max_height, float height_exponent, Vector2 noise_offset, float detail) {
    world_noise = Object::cast_to<FastNoiseLite>(Ref<Noise>(proc_level->noise).operator->());
    Array surface_array = Array();
    surface_array.resize(Mesh::ARRAY_MAX);

    SurfaceTool *surface_tool = memnew(SurfaceTool);
    surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

    mesh_instance->set_mesh(memnew(ArrayMesh));
    
    int this_row = 0;
    int prev_row = 0;
    int row = 0;
    int column = 0;
    int point = 0;

    for (int i = 0; i < square_size; ++i) {
        float v = i / square_size;
        for (int j = 0; j < square_size; ++j) {
            float u = j / square_size;
            float x = column * int_dis;
            column++;
            float z = row * int_dis;
            float y = world_noise->get_noise_2d((x + noise_offset.x) * detail, (z + noise_offset.y) * detail) * pow(max_height, height_exponent);

            Vector3 vert = Vector3(x, y, z);
            proc_level->mesh_verts.append(vert + get_global_position());
            point++;

            surface_tool->set_uv(Vector2(u, v));
            surface_tool->set_smooth_group(SmoothIndex);
            surface_tool->add_vertex(vert);

            if (i > 0 && j > 0) {
                if (UseSmooth) {surface_tool->set_smooth_group(SmoothIndex);}
                surface_tool->add_index(prev_row + j - 1);
                if (UseSmooth) {surface_tool->set_smooth_group(SmoothIndex);}
                surface_tool->add_index(prev_row + j);
                if (UseSmooth) {surface_tool->set_smooth_group(SmoothIndex);}
                surface_tool->add_index(this_row + j - 1);
                if (UseSmooth) {surface_tool->set_smooth_group(SmoothIndex);}
                surface_tool->add_index(prev_row + j);
                if (UseSmooth) {surface_tool->set_smooth_group(SmoothIndex);}
                surface_tool->add_index(this_row + j);
                if (UseSmooth) {surface_tool->set_smooth_group(SmoothIndex);}
                surface_tool->add_index(this_row + j - 1);
            }
        }
        prev_row = this_row;
        this_row = point;
        row++;
        column = 0;
    }
    surface_tool->index();
    surface_tool->optimize_indices_for_cache();
    surface_tool->generate_normals();
    surface_tool->generate_tangents();

    mesh_instance->set_mesh(surface_tool->commit());
    mesh_instance->set_surface_override_material(0, proc_level->get_terrain_material());
    Ref<ConcavePolygonShape3D> shape;
    shape = mesh_instance->get_mesh()->create_trimesh_shape();
    collision_shape->set_shape(shape);
    if (proc_level->generate_navigation) {
        nav_region->bake_navigation_mesh(true);
    }
}
