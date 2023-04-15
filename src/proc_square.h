#ifndef GODOT_PROC_SQUARE_H
#define GODOT_PROC_SQUARE_H

#ifdef WIN32
#include <windows.h>
#endif


#include "proc.h"

#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/navigation_region3d.hpp>
#include <godot_cpp/classes/navigation_mesh.hpp>
#include <godot_cpp/classes/fast_noise_lite.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class ProcSquare : public StaticBody3D {
    GDCLASS(ProcSquare, StaticBody3D);

    FastNoiseLite *world_noise = nullptr;
    CollisionShape3D *collision_shape = memnew(CollisionShape3D);
    MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
    PackedVector3Array mesh_verts = PackedVector3Array();
    int SmoothIndex = 2;
    bool UseSmooth = true;
    NavigationRegion3D *nav_region = memnew(NavigationRegion3D);
    Ref<NavigationMesh> nav_mesh = nullptr;

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    class Proc *proc_level = nullptr;

public:
    ProcSquare();
    ~ProcSquare();

    void generate(float square_size, float int_dis, float max_height, float height_exponent, Vector2 noise_offset, float detail);
};


#endif //GODOT_PROC_SQUARE_H
