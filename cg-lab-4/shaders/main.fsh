#version 420 core
in vec3 real_pos;
in vec3 color;
in vec3 real_normal;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 view_pos;

uniform bool ambt_bool;
uniform bool diff_bool;
uniform bool spec_bool;

out vec4 frag_color;

void main()
{
    float ambt = ambt_bool ? 0.1f : 0;
    vec3  light_dir   = normalize(real_pos - light_pos);
    float dist = distance(real_pos, light_pos);

    float diff = 0;
    if (diff_bool) {
        diff = max(dot(real_normal, light_dir), 0.0f);
        diff /= dist;
    }

    float spec = 0;
    if (spec_bool) {
        vec3  view_dir    = normalize(view_pos - real_pos);
        vec3  reflect_dir = reflect(-light_dir, real_normal);
        spec = dot(view_dir, reflect_dir);
        spec = spec > 0.0f ? spec : 0;
        spec = pow(spec, 32);
        spec /= dist;
    }

    vec3 result = (ambt + diff + spec) * light_color * color;
    frag_color = vec4(result, 1.0f);
}
