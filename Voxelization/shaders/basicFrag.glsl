#version 430 core
out vec4 FragColor;



uniform float time;
uniform vec3 rayOrigin;
uniform int number;

in vec3 col;

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Hit{
    float hitDist;
    vec3 worldPos;
    vec3 normalPos;
    int objIndex;
};

layout (std140, binding = 1) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

layout(std430, binding = 5) buffer buf
{
    vec4 S[200][200][200];
};

vec3 random(vec2 st) {
    vec3 rand;
    rand.x = fract(sin(time*dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
    rand.y = fract(sin(rand.x)*55751.6475574);
    rand.z = fract(sin(rand.y)*68446.4894964);
    return rand;
}

Hit miss(Ray ray){
    Hit hit;
    hit.hitDist = -1;
    return hit;
};

bool getVoxel(ivec3 c) {
    //if (0<c.x && c.x<number && 0<c.y && c.y<number && 0<c.z && c.z<number)
	return S[c.x][c.y][c.z].w == 1;
}

void main()
{
    vec3 color = vec3(0.0f);
    vec2 pos = (gl_FragCoord.xy/ vec2(1280,720))*2.0f-1.0f;
    vec4 target = inverse(projection) * vec4(pos,1.0f,1.0f);
    vec3 rayDir = vec3(inverse(view)*vec4(normalize(target.xyz/target.w),0));
    pos.x*=1.77f;
    Ray ray;
    ray.origin = rayOrigin;
    ray.direction = rayDir;
    int bounces = 0;

    bvec3 mask;

    ivec3 map = ivec3(floor(ray.origin+0.f));
    vec3 deltaDist = abs(1/ray.direction);
    ivec3 _step = ivec3(sign(rayDir));
    vec3 sideDist = (sign(ray.direction) * (vec3(map)-ray.origin) + (sign(ray.direction)*0.5)+0.5)*deltaDist;

    bool found = false;

    for(int i = 0; i< 10000; i++){
        if (sideDist.x < sideDist.y) {
			if (sideDist.x < sideDist.z) {
				sideDist.x += deltaDist.x;
				map.x += _step.x;
				mask = bvec3(true, false, false);
			}
			else {
				sideDist.z += deltaDist.z;
				map.z += _step.z;
				mask = bvec3(false, false, true);
			}
		}
		else {
			if (sideDist.y < sideDist.z) {
				sideDist.y += deltaDist.y;
				map.y += _step.y;
				mask = bvec3(false, true, false);
			}
			else {
				sideDist.z += deltaDist.z;
				map.z += _step.z;
				mask = bvec3(false, false, true);
			}
		}
        if(getVoxel(map)){
            found = true;
            break;
        }
    }
    if(found){
        if (mask.x) {
		color = vec3(0.5);
	}
	    if (mask.y) {
		color = vec3(1.0);
	}
	    if (mask.z) {
		color = vec3(0.75);
	}
    }
    

    FragColor = vec4(color , 1.0f);
    
}