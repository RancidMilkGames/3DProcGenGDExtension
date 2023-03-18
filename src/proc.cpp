#include "proc.h"


#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/world3d.hpp>

using namespace godot;

Proc::Proc() {
}

Proc::~Proc() {
    memfree(rand);
}

void Proc::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {

        } break;
        case NOTIFICATION_ENTER_WORLD: {

        } break;

        case NOTIFICATION_READY: {

        } break;
    }
}

void Proc::_bind_methods()
{

    ClassDB::bind_method(D_METHOD("generate"), &Proc::generate);
    ClassDB::bind_method(D_METHOD("set_vertices_array", "p_array"), &Proc::set_vertices);
    ClassDB::bind_method(D_METHOD("get_vertices_array"), &Proc::get_vertices);
    ClassDB::bind_method(D_METHOD("set_terrain_material", "p_material"), &Proc::set_terrain_material);
    ClassDB::bind_method(D_METHOD("get_terrain_material"), &Proc::get_terrain_material);
    ClassDB::bind_method(D_METHOD("set_rows", "p_rows"), &Proc::set_rows);
    ClassDB::bind_method(D_METHOD("get_rows"), &Proc::get_rows);
    ClassDB::bind_method(D_METHOD("set_columns", "p_columns"), &Proc::set_columns);
    ClassDB::bind_method(D_METHOD("get_columns"), &Proc::get_columns);
    ClassDB::bind_method(D_METHOD("set_indices_distance", "p_distance"), &Proc::set_indices_distance);
    ClassDB::bind_method(D_METHOD("get_indices_distance"), &Proc::get_indices_distance);
    ClassDB::bind_method(D_METHOD("set_max_height", "p_max_height"), &Proc::set_max_height);
    ClassDB::bind_method(D_METHOD("get_max_height"), &Proc::get_max_height);
    ClassDB::bind_method(D_METHOD("set_height_exponent", "p_height_exponent"), &Proc::set_height_exponent);
    ClassDB::bind_method(D_METHOD("get_height_exponent"), &Proc::get_height_exponent);
    ClassDB::bind_method(D_METHOD("set_detail", "p_detail"), &Proc::set_detail);
    ClassDB::bind_method(D_METHOD("get_detail"), &Proc::get_detail);
    ClassDB::bind_method(D_METHOD("set_generate_navigation", "p_generate_navigation"), &Proc::set_generate_navigation);
    ClassDB::bind_method(D_METHOD("get_generate_navigation"), &Proc::get_generate_navigation);
    ClassDB::bind_method(D_METHOD("set_noise_texture", "p_noise_texture"), &Proc::set_noise_texture);
    ClassDB::bind_method(D_METHOD("get_noise_texture"), &Proc::get_noise_texture);
    ClassDB::bind_method(D_METHOD("noise_ready"), &Proc::noise_ready);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "terrain_material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_terrain_material", "get_terrain_material");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR3_ARRAY, "mesh_vertices", PROPERTY_HINT_ARRAY_TYPE), "set_vertices_array", "get_vertices_array");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "rows", PROPERTY_HINT_RANGE, "1,100,1"), "set_rows", "get_rows");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "columns", PROPERTY_HINT_RANGE, "1,100,1"), "set_columns", "get_columns");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "indices_distance", PROPERTY_HINT_RANGE, "1,500,.1"), "set_indices_distance", "get_indices_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_height", PROPERTY_HINT_RANGE, ".1,1000,.1"), "set_max_height", "get_max_height");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "height_exponent", PROPERTY_HINT_RANGE, "1,500,.1"), "set_height_exponent", "get_height_exponent");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "detail", PROPERTY_HINT_RANGE, ".01,100,.01"), "set_detail", "get_detail");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "generate_navigation"), "set_generate_navigation", "get_generate_navigation");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "noise_texture", PROPERTY_HINT_RESOURCE_TYPE, "NoiseTexture2D"), "set_noise_texture", "get_noise_texture");

    ADD_SIGNAL(MethodInfo("generated"));
}

void Proc::generate() {
    if (terrain_material == nullptr) {
        terrain_material = Ref<Material>(memnew(Material));
    }
    if (noise_texture == nullptr) {
        noise_texture = Ref<NoiseTexture2D>(memnew(NoiseTexture2D));
    }
    if (Ref<NoiseTexture2D>(noise_texture)->get_noise() == nullptr) {
        noise = Ref<FastNoiseLite>(memnew(FastNoiseLite()));
        Ref<NoiseTexture2D> noise_texture2d = Ref<NoiseTexture2D>(noise_texture);
        Ref<FastNoiseLite>(noise)->set_noise_type(FastNoiseLite::TYPE_PERLIN);
        noise_texture2d->set_seamless(true);
        noise_texture2d->set_as_normal_map(true);
        noise_texture2d->set_bump_strength(8.0f);
        noise_texture2d->set_normalize(false);
        noise_texture2d->set_noise(Ref<Noise>(noise));
    } else {
        noise = Ref<NoiseTexture2D>(noise_texture)->get_noise().operator->();
    }

    FastNoiseLite *noise_lite = Object::cast_to<FastNoiseLite>(Ref<Noise>(noise).operator->());
    noise_lite->set_seed(Ref<RandomNumberGenerator>(rand)->randi_range(0, 1000000));
    Callable &callable = Callable(this, "noise_ready");
    Ref<NoiseTexture2D>(noise_texture)->connect("changed", callable, Object::CONNECT_ONE_SHOT);

    RID default_3d_map_rid = get_world_3d()->get_navigation_map();
    NavigationServer3D::get_singleton()->map_set_edge_connection_margin(default_3d_map_rid, 5.0f);
}

void Proc::noise_ready() {
    square_size = Vector2((total_size - 1) * indices_distance, (total_size - 1) * indices_distance);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ProcSquare *square = memnew(ProcSquare);
            add_child(square);
            Vector2 offset = Vector2((total_size - 1) * indices_distance * i, (total_size - 1) * indices_distance * j);
            square->set_global_position(square->get_global_position() + Vector3(offset.x, 0, offset.y));
            square->generate(total_size, indices_distance, max_height, height_exponent, offset, detail);
            square->set_name("Square " + String::num(i) + " " + String::num(j));
            proc_gen_squares.append(square);
        }
    }
    emit_signal("generated");
}

void Proc::set_vertices(PackedVector3Array p_vertices) {
    mesh_verts = p_vertices;
}

PackedVector3Array Proc::get_vertices() {
    return mesh_verts;
}

void Proc::set_terrain_material(Ref<Material> p_material) {
    terrain_material = p_material;
}

Ref<Material> Proc::get_terrain_material() {
    return terrain_material;
}

void Proc::set_rows(int p_rows) {
    rows = p_rows;
    total_size = rows * columns;
}

int Proc::get_rows() {
    return rows;
}

void Proc::set_columns(int p_columns) {
    columns = p_columns;
    total_size = rows * columns;
}

int Proc::get_columns() {
    return columns;
}

void Proc::set_indices_distance(float p_indices_distance) {
    indices_distance = p_indices_distance;
}

float Proc::get_indices_distance() {
    return indices_distance;
}

void Proc::set_max_height(float p_max_height) {
    max_height = p_max_height;
}

float Proc::get_max_height() {
    return max_height;
}

void Proc::set_detail(float p_detail) {
    detail = p_detail;
}

float Proc::get_detail() {
    return detail;
}

void Proc::set_height_exponent(float p_height_exponent) {
    height_exponent = p_height_exponent;
}

float Proc::get_height_exponent() {
    return height_exponent;
}

void Proc::set_generate_navigation(bool p_generate_navigation) {
    generate_navigation = p_generate_navigation;
}

bool Proc::get_generate_navigation() {
    return generate_navigation;
}

void Proc::set_noise_texture(Ref<NoiseTexture2D> p_noise_texture) {
    noise_texture = p_noise_texture;
}

Ref<NoiseTexture2D> Proc::get_noise_texture() {
    return noise_texture;
}





