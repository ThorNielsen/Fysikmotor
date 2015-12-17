#include "include/rigid_body.hpp"
#include "include/debug.hpp"

rigid_body::rigid_body(rigid_body&& o)
{
    m_mesh = std::move(o.m_mesh);
    m_vertices = std::move(o.m_vertices);
    m_axes = std::move(o.m_axes);
    m_inertia = std::move(o.m_inertia);
    m_iinertia = std::move(o.m_iinertia);
    m_wiinertia = std::move(o.m_wiinertia);
    m_apos = std::move(o.m_apos);
    m_avel = std::move(o.m_avel);
    m_radii = std::move(o.m_radii);
    m_pos = std::move(o.m_pos);
    m_vel = std::move(o.m_vel);
    m_force = std::move(o.m_force);
    m_mom = std::move(o.m_mom);
    m_amom = std::move(o.m_amom);
    m_edge_begin = std::move(o.m_edge_begin);
    m_mass = std::move(o.m_mass);
    m_inv_mass = std::move(o.m_inv_mass);
}

// Control rigid body in non-physical way
void rigid_body::input(window& w)
{
    float force = m_mass * 10.0f;
    float torque = 0.3f;

    if (w.keydown(GLFW_KEY_LEFT))
    {
        m_force.x -= force;
    }
    if (w.keydown(GLFW_KEY_RIGHT))
    {
        m_force.x += force;
    }
    if (w.keydown(GLFW_KEY_UP))
    {
        m_force.z -= force;
    }
    if (w.keydown(GLFW_KEY_DOWN))
    {
        m_force.z += force;
    }
    if (w.keydown(GLFW_KEY_LEFT_SHIFT))
    {
        m_force.y += force*2;
        if (w.keydown(GLFW_KEY_6)) m_force.y *= 10;
    }
    if (w.keydown(GLFW_KEY_LEFT_CONTROL))
    {
        m_force.y -= force*2;
    }
    if (w.keydown(GLFW_KEY_Q))
    {
        rotate({0,1,0}, 0.01f);
    }
    if (w.keydown(GLFW_KEY_E))
    {
        rotate({0,1,0}, -0.01f);
    }
    if (w.keydown(GLFW_KEY_A))
    {
        rotate({0,0,1}, 0.01f);
    }
    if (w.keydown(GLFW_KEY_D))
    {
        rotate({0,0,1}, -0.01f);
    }
    if (w.keydown(GLFW_KEY_W))
    {
        rotate({1,0,0}, 0.01f);
    }
    if (w.keydown(GLFW_KEY_S))
    {
        rotate({1,0,0}, -0.01f);
    }
}

void rigid_body::add_axe(vec3 axe)
{
    for (size_t i = m_edge_begin; i < m_axes.size(); ++i)
    {
        // If same direction
        if (zero_vector(cross(axe, m_axes[i]))) return;
    }
    m_axes.push_back(normalize(axe));
}


void rigid_body::add_vertex(vec3 k)
{
    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        if (same_position(m_vertices[i], k)) return;
    }
    m_vertices.push_back(k);
}

void rigid_body::optimise()
{
    m_edge_begin = 0;
    const std::vector<vertex>& avert = m_mesh.verts();
    const std::vector<GLuint>& ai = m_mesh.indices();
    size_t avertsz = avert.size();
    size_t aisz = ai.size();
    for (size_t i = 0; i < aisz; i += 3)
    {
        vec3 normal = cross(avert[ai[i+1]].position - avert[ai[i]].position,
                            avert[ai[i+2]].position - avert[ai[i]].position);
        add_axe(normal);
    }
    m_edge_begin = m_axes.size();

    for (size_t i = 0; i < aisz; i += 3)
    {
        vec3 p0 = avert[ai[i]].position;
        vec3 p1 = avert[ai[i+1]].position;
        vec3 p2 = avert[ai[i+2]].position;
        add_axe(p1-p0);
        add_axe(p2-p1);
        add_axe(p0-p2);
    }
    for (size_t i = 0; i < avertsz; ++i)
    {
        add_vertex(avert[i].position);
    }
}

// Returns magnitude of interpenetration depth.
// A value > 0 means not penetrating.
float separating_axe(vec3 axis, mat3 rot, vec3 translation,
                     const std::vector<vec3>& verts1,
                     const std::vector<vec3>& verts2)
{
    float a0 = FLT_MAX, a1 = -FLT_MAX, b0 = a0, b1 = a1;
    for (size_t i = 0; i < verts1.size(); ++i)
    {
        float tmp = dot(verts1[i], axis);
        a0 = std::min(tmp, a0);
        a1 = std::max(tmp, a1);
    }

    for (size_t i = 0; i < verts2.size(); ++i)
    {
        float tmp = dot(verts2[i] * rot + translation, axis);
        b0 = std::min(tmp, b0);
        b1 = std::max(tmp, b1);
    }
    if (a1 < b0) return a1 - b0;
    if (b1 < a0) return b1 - a0;
    if ((b0 < a1||flt_eq(b0,a1)) && a0 < b0 && a1 < b1) return a1 - b0;
    if ((a0 < b1||flt_eq(a0,b1)) && b0 < a0 && b1 < a1) return b1 - a0;
    if (b1 <= a1 && a0 <= b0) return std::min(a1-b0, b1-a0);
    if (a1 <= b1 && b0 <= a0) return std::min(b1-a0, a1-b0);
    // This REALLY shouldn't be possible as all cases are covered.
    std::cout << a0 << " -> " << a1 << std::endl;
    std::cout << b0 << " -> " << b1 << std::endl;
    write(axis, "Axis");
    write(rot, "Rot");
    class not_possible {};
    not_possible very_bad_error;
    throw very_bad_error;
}

// to_convex_polygon should be called with verts containing the vertices
// of the convex polygon in any order, verts will be returned containing
// the vertices of the polygon in the correct order (edge from verts[0]
// to verts[1], from verts[n-1] to verts[n] and verts[n] to verts[0])
// The vertices must be coplanar (or very near coplanar).
void to_convex_polygon(std::vector<vec3>& verts)
{
    std::vector<vec3> sorted;
    sorted.reserve(verts.size());
    sorted.push_back(verts[0]);
    float min_dist = FLT_MAX;
    vec3 prev;
    for (size_t i = 1; i < verts.size(); ++i)
    {
        float f = dist_sqr(verts[0], verts[i]);
        if (f < min_dist)
        {
            min_dist = f;
            prev = verts[i];
        }
    }
    sorted.push_back(prev);
    size_t num_sorted = 2;
    while (num_sorted < verts.size())
    {
        float ca = -1.0f;
        size_t idx = 1;
        for (size_t i = 1; i < verts.size(); ++i)
        {
            if (zero_vector(cross(sorted[num_sorted-1]-sorted[num_sorted-2],
                                verts[i] - sorted[num_sorted-1]))) continue;
            float c = cos_angle(sorted[num_sorted-1]-sorted[num_sorted-2],
                                verts[i] - sorted[num_sorted-1]);
            if (c > ca)
            {
                ca = c;
                idx = i;
            }
        }
        sorted.push_back(verts[idx]);
        ++num_sorted;
    }
    std::swap(verts, sorted);
}

void points_inside(std::vector<vec3>& points,
                   const std::vector<vec3>& p1,
                   const std::vector<vec3>& p2,
                   vec3 n)
{
    for (size_t i = 0; i < p1.size(); ++i)
    {
        bool breaking = false;
        bool last_sign = dot(cross(p2[1]-p2[0], p1[i] - p2[0]), n) <= 0;
        for (size_t j = 2; j < p2.size(); ++j)
        {
            if ((dot(cross(p2[j]-p2[j-1], p1[i] - p2[j-1]), n) <= 0)
                != last_sign)
            {
                breaking = true;
                break;
            }
        }
        if (breaking) continue;
        if ((dot(cross(p2[0]-p2[p2.size()-1], p1[i] - p2[p2.size()-1]), n) <= 0)
            == last_sign)
        {
            points.push_back(p1[i]);
        }
    }
}


vec3 center_of_mass(const std::vector<vec3>& points)
{
    vec3 tot_point{0.0f,0.0f,0.0f};
    vec3 o = points[0];
    size_t psz = points.size() - 1;
    float tot_area = 0;
    for (size_t i = 1; i < psz; ++i)
    {
        float area = length(cross(points[i] - o, points[i+1] - o)) / 2.0f;
        tot_area += area;
        tot_point += ((o + points[i] + points[i+1]) / 3.0f) * area;
    }
    tot_point /= tot_area;
    return tot_point;
}


// Returns middle point of two faces intersecting when projected at
// the normal n.
vec3 facefacepoint(const std::vector<vec3>& verts1,
                       const std::vector<vec3>& verts2,
                       vec3 n)

{
    std::vector<vec3> p1 = verts1;
    to_convex_polygon(p1);
    std::vector<vec3> p2 = verts2;
    to_convex_polygon(p2);
    std::vector<vec3> points;
    points_inside(points, p1, p2, n);
    points_inside(points, p2, p1, n);
    for (size_t i = 0; i < p1.size(); ++i)
    {
        for (size_t j = 0; j < p2.size(); ++j)
        {
            float t, s;
            line n{p1[i], p1[(i+1)%p1.size()]-p1[i]};
            line m{p2[j], p2[(j+1)%p2.size()]-p2[j]};
            line_line(n, m, t, s);
            if (0 <= t && t <= 1 && 0 <= s && s <= 1)
            {
                points.push_back((n.p+t*n.r+m.p+s*m.r) / 2.0f);
            }
        }
    }
    to_convex_polygon(points);
    vec3 p = {0,0,0};
    for (auto& i : points)
    {
        p += i;
    }
    p /= points.size();
    return p;
}

vec3 edgefacepoint(std::vector<vec3> verts, vec3 p1, vec3 p2)
{
    to_convex_polygon(verts);
    line n{p1, p2-p1};
    bool nondescriptive_name = false;
    for (size_t i = 0; i < verts.size(); ++i)
    {
        float t, s;
        line m{verts[i], verts[(i+1)%verts.size()]-verts[i]};
        if (zero_vector(cross(n.r, m.r)))
        {
            if (zero_vector(cross(m.p-p1, m.r)))
            {
                // Edge coincide with one of the edges of the face
                float l1 = length_sqr(p2-p1);
                float l2 = length_sqr(p2-m.p);
                float l3 = length_sqr(p2-m.p-m.r);
                float l4 = length_sqr(m.r);
                float l5 = length_sqr(p1-m.p-m.r);
                float l6 = length_sqr(p1-m.p);
                if (l1 <= l2 && l1 <= l3 && l1 <= l4 &&
                    l1 <= l5 && l1 <= l6) return (p1+p2) * 0.5f;
                if (l2 <= l1 && l2 <= l3 && l2 <= l4 &&
                    l2 <= l5 && l2 <= l6) return (m.p+p2) * 0.5f;
                if (l3 <= l1 && l3 <= l2 && l3 <= l4 &&
                    l3 <= l5 && l3 <= l6) return (m.p+m.r+p2) * 0.5f;
                if (l4 <= l1 && l4 <= l2 && l4 <= l3 &&
                    l4 <= l5 && l4 <= l6) return (m.p+m.p+m.r) * 0.5f;
                if (l5 <= l1 && l5 <= l2 && l5 <= l3 &&
                    l5 <= l4 && l5 <= l6) return (p1+m.p+m.r) * 0.5f;
                if (l6 <= l1 && l6 <= l2 && l6 <= l3 &&
                    l6 <= l4 && l6 <= l5) return (p1+m.p) * 0.5f;
                class blow{};
                blow up;
                throw up;
            }
            continue;
        }
        line_line(n, m, t, s);
        if (0 <= t && t <= 1 && 0 <= s && s <= 1)
        {
            if (!nondescriptive_name)
            {
                p1 = (n.p+t*n.r+m.p+s*m.r) / 2.0f;
            }
            else
            {
                p2 = (n.p+t*n.r+m.p+s*m.r) / 2.0f;
            }
        }
    }
    return (p1+p2) / 2.0f;
}


collision_info ci_separating_axe(vec3 axis, mat3 rot, vec3 translation,
                     const std::vector<vec3>& verts1,
                     const std::vector<vec3>& verts2)
{
    std::vector<vec3> amin;
    std::vector<vec3> amax;
    std::vector<vec3> bmin;
    std::vector<vec3> bmax;
    float a0 = FLT_MAX, a1 = -FLT_MAX, b0 = a0, b1 = a1;
    for (size_t i = 0; i < verts1.size(); ++i)
    {
        float tmp = dot(verts1[i], axis);
        if (flt_eq(tmp, a0))
        {
            amin.push_back(verts1[i]);
        }
        else if (tmp < a0)
        {
            amin.clear();
            amin.push_back(verts1[i]);
            a0 = tmp;
        }

        if (flt_eq(tmp, a1))
        {
            amax.push_back(verts1[i]);
        }
        else if (tmp > a1)
        {
            amax.clear();
            amax.push_back(verts1[i]);
            a1 = tmp;
        }
    }

    for (size_t i = 0; i < verts2.size(); ++i)
    {
        vec3 point = verts2[i] * rot + translation;
        float tmp = dot(point, axis);
        if (flt_eq(tmp, b0))
        {
            bmin.push_back(point);
        }
        else if (tmp < b0)
        {
            bmin.clear();
            bmin.push_back(point);
            b0 = tmp;
        }
        if (flt_eq(tmp, b1))
        {
            bmax.push_back(point);
        }
        else if (tmp > b1)
        {
            bmax.clear();
            bmax.push_back(point);
            b1 = tmp;
        }
    }

    // aaaaaaaa|bbbbbbbb <-- a collides with b like this.
    // Collision a-b:
    bool cab;
    if ((b0 < a1||flt_eq(b0,a1)) && a0 < b0 && a1 < b1) cab = true;
    if ((a0 < b1||flt_eq(a0,b1)) && b0 < a0 && b1 < a1) cab = false;
    if (b1 <= a1 && a0 <= b0) cab = a1-b0 < b1-a0;
    if (a1 <= b1 && b0 <= a0) cab = b1-a0 > a1-b0;
    collision_info ci;
    ci.normal = cab?-axis:axis;
    const std::vector<vec3>& pnt0 = cab ? amax : bmax;
    const std::vector<vec3>& pnt1 = cab ? bmin : amin;
    if (pnt0.size() > 2) // Face
    {
        if (pnt1.size() > 2) // Face-face
        {
            ci.point = facefacepoint(pnt0, pnt1, ci.normal);
        }
        else if (pnt1.size() == 2) // Face-edge
        {
            ci.point = edgefacepoint(pnt0, pnt1[0], pnt1[1]);
        }
        else if (pnt1.size() == 1) // Face-vertex
        {
            ci.point = pnt1[0];
        }
    }
    else if (pnt0.size() == 2) // Edge
    {
        if (pnt1.size() > 2) // Edge-face
        {
            ci.point = edgefacepoint(pnt1, pnt0[0], pnt0[1]);
        }
        else if (pnt1.size() == 2) // Edge-edge
        {
            vec3 pa = pnt0[0];
            vec3 pb = pnt0[1];
            vec3 pc = pnt1[0];
            vec3 pd = pnt1[1];
            line a {pa, pb - pa};
            line b {pc, pd - pc};
            if (zero_vector(cross(a.r, b.r)))
            {
                ci.point = (pa + pb + pc + pd) * 0.25f;
            }
            else ci.point = closest_point_between(a, b);
        }
        else if (pnt1.size() == 1) // Edge-vertex
        {
            ci.point = pnt1[0];
        }
    }
    else if (pnt0.size() == 1) // Vertex
    {
        ci.point = pnt0[0];
    }
    return ci;
}

bool rigid_body::intersect(rigid_body& o)
{
    if (m_mass == IMMOBILE && o.m_mass == IMMOBILE) return false;
    mat3 rot = transpose(o.m_apos) * m_apos;
    vec3 trans = o.m_pos - m_pos;
    trans = trans * m_apos;
    float max_dist = FLT_MAX;
    int closest_axis; // 0 -> face in this object, i1 points to index in m_axes
                      // 1 -> face in o, i2 points to index in o.m_axes
                      // 2 -> edge pair, i1 points to this object's edge,
                      //       i2 points to other object's edge.
    size_t i1, i2;
    for (size_t i = 0; i < m_edge_begin; ++i)
    {
        float s = separating_axe(m_axes[i], rot, trans, m_vertices, o.m_vertices);
        if (s < 0.0f) return false;
        if (s < max_dist)
        {
            closest_axis = 0;
            i1 = i;
            max_dist = s;
        }
    }
    for (size_t i = 0; i < o.m_edge_begin; ++i)
    {
        vec3 axe = o.m_axes[i] * rot;
        float s = separating_axe(axe, rot, trans, m_vertices, o.m_vertices);
        if (s < 0.0f) return false;
        if (s < max_dist)
        {
            closest_axis = 1;
            i2 = i;
            max_dist = s;
        }
    }
    for (size_t i = m_edge_begin; i < m_axes.size(); ++i)
    {
        for (size_t j = o.m_edge_begin; j < o.m_axes.size(); ++j)
        {
            vec3 axe = cross(m_axes[i], o.m_axes[j] * rot);
            if (zero_vector(axe)) continue;
            float s = separating_axe(normalize(axe),
                                     rot,
                                     trans,
                                     m_vertices,
                                     o.m_vertices);
            if (s < 0.0f) return false;
            if (s < max_dist)
            {
                closest_axis = 2;
                i1 = i;
                i2 = j;
                max_dist = s;
            }
        }
    }
    vec3 colliding_axe;
    if (closest_axis == 0) colliding_axe = m_axes[i1];
    if (closest_axis == 1) colliding_axe = o.m_axes[i2] * rot;
    if (closest_axis == 2)
        colliding_axe = normalize(cross(m_axes[i1], o.m_axes[i2] * rot));
    collision_info ci = ci_separating_axe(colliding_axe,
                                          rot,
                                          trans,
                                          m_vertices,
                                          o.m_vertices);

    // Move objects back to the moment of collision
    float tot_mass = m_mass + o.m_mass;
    m_pos += (m_mass / tot_mass) * max_dist * ci.normal * transpose(m_apos);
    o.m_pos -= (o.m_mass / tot_mass) * max_dist * (ci.normal*transpose(rot)*transpose(o.m_apos));

    trans = o.m_pos - m_pos;
    trans = trans * m_apos;

    ci = ci_separating_axe(colliding_axe,
                           rot,
                           trans,
                           m_vertices,
                           o.m_vertices);

    vec3 ap = ci.point;
    vec3 bp = (ci.point-trans)*transpose(rot);
/*
    vec3 vel = m_vel * m_apos - o.m_vel * rot + cross(m_avel, ap) * m_apos - cross(o.m_avel, bp) * rot;

    float num =- dot(vel, ci.normal);
    float denom = (dot(ci.normal, ci.normal) * (m_inv_mass + o.m_inv_mass)) +
        dot(cross(m_iinertia * cross(ap, ci.normal), ap) +
        cross(o.m_iinertia * cross(bp, ci.normal), bp),
             ci.normal);
    vec3 impulse = (num / denom) * ci.normal;
    m_vel += m_inv_mass * impulse * transpose(m_apos);
    o.m_vel -= o.m_inv_mass * impulse * transpose(rot) * transpose(o.m_apos);

    m_amom -= cross(ap, impulse * transpose(m_apos));
    o.m_amom += cross(bp, impulse * transpose(rot) * transpose(o.m_apos));
    m_avel += m_iinertia * (m_amom * m_apos);
    o.m_avel -= o.m_iinertia * (o.m_amom * o.m_apos);/*/

    ci.normal = ci.normal * transpose(m_apos);
    ap = ap * transpose(m_apos);
    bp = bp * transpose(m_apos);
    ap = ci.point * transpose(m_apos);
    bp = ci.point * transpose(m_apos) - trans;


    vec3 vel = o.m_vel
    + cross(bp, o.m_avel)
    - m_vel
    - cross(ap, m_avel);
    float num = 1 * dot(vel, ci.normal);
    float denom0 = (m_inv_mass + o.m_inv_mass) * dot(ci.normal, ci.normal);
    float denom1 = dot(cross(m_wiinertia * cross(ap, ci.normal), ap) +
                    cross(o.m_wiinertia * cross(bp, ci.normal), bp),
                       ci.normal);
    vec3 impulse = (num / (denom0 + denom1)) * ci.normal;
    m_vel += (m_inv_mass * impulse);
    o.m_vel -= (o.m_inv_mass * impulse);
    m_amom += cross(impulse, ap);
    o.m_amom -= cross(impulse, bp);

    m_avel += m_wiinertia * (m_amom);
    o.m_avel += o.m_wiinertia * (o.m_amom);
//*/
    //volatile int sadfhoi = 1/0;
    //write(colliding_axe, "normal");
    return true;
}

void rigid_body::update(float dt)
{
    ///write(m_pos, "m_pos");
    m_pos += m_vel * dt;
    m_vel += m_force * m_inv_mass * dt;
    if (flt_eq(m_vel.x/50, 0)) m_vel.x = 0;
    if (flt_eq(m_vel.y/50, 0)) m_vel.y = 0;
    if (flt_eq(m_vel.z/50, 0)) m_vel.z = 0;
    m_force = {0, 0, 0};
    m_force.y -= 9.82f * m_mass;

    m_apos += dt * skew(m_avel) * m_apos;
    orthonormalise(m_apos);
    m_wiinertia = m_apos * m_iinertia * transpose(m_apos);
    m_amom *= 0.99f;
    m_avel = m_wiinertia * m_amom;
}
