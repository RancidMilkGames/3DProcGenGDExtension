#ifndef PROC_CLASS_H
#define PROC_CLASS_H

#ifdef WIN32
#include <windows.h>
#endif

#include "proc_square.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/noise_texture2d.hpp>
#include <godot_cpp/classes/fast_noise_lite.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/material.hpp>

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class Proc : public Node3D {
    GDCLASS(Proc, Node3D);

public:
    int total_size = 64;
    int rows = 8;
    int columns = 8;
    float indices_distance = 5;
    float max_height = 150;
    float height_exponent = 1;
    float detail = .2f;
    Ref<Material> terrain_material = nullptr;
    bool generate_navigation = false;
    Ref<FastNoiseLite> noise = nullptr;
    Ref<NoiseTexture2D> noise_texture = nullptr;

    Array proc_gen_squares = Array();
    Vector2 square_size;

    PackedVector3Array mesh_verts = PackedVector3Array();
    RandomNumberGenerator *rand = memnew(RandomNumberGenerator);



private:

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    Proc();
    ~Proc();

    void generate();

    void set_vertices(PackedVector3Array p_vertices);
    PackedVector3Array get_vertices();

    void set_terrain_material(Ref<Material> p_material);
    Ref<Material> get_terrain_material();

    void set_rows(int p_rows);
    int get_rows();

    void set_columns(int p_columns);
    int get_columns();

    void set_indices_distance(float p_indices_distance);
    float get_indices_distance();

    void set_max_height(float p_max_height);
    float get_max_height();

    void set_detail(float p_detail);
    float get_detail();

    void set_height_exponent(float p_height_exponent);
    float get_height_exponent();

    void set_generate_navigation(bool p_generate_navigation);
    bool get_generate_navigation();

    void set_noise_texture(Ref<NoiseTexture2D> p_custom_noise_texture);
    Ref<NoiseTexture2D> get_noise_texture();

    void noise_ready();
};

#endif // PROC_CLASS_H