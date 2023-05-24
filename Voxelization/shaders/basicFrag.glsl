#version 430 core
out vec4 FragColor;


// from cpu to gpu
uniform float time;
uniform vec3 rayOrigin;
uniform int number;
uniform int scroll;
uniform int mouse;

struct Ray{
    vec3 origin;
    vec3 direction;
};


layout (std140, binding = 1) uniform Matrices
{
    // matrices for camera
    mat4 projection;
    mat4 view;
};

layout(std430, binding = 5) buffer buf
{
    // voxel grid
    float S[];
};

bool getVoxel(ivec3 c) {
    // check for voxel in grid
    if (0<c.x && c.x<number && 0<c.y && c.y<number && 0<c.z && c.z<number)
	    return S[number*number*c.z+number*c.y+c.x]==1;
}

void main()
{
    // variables init
    vec3 lightSource = vec3(number/2,500,number/2);
    vec2 pos = (gl_FragCoord.xy/ vec2(1280,720))*2.0f-1.0f;
    vec4 target = inverse(projection) * vec4(pos,1.0f,1.0f);
    vec3 rayDir = vec3(inverse(view)*vec4(normalize(target.xyz/target.w),0));
    pos.x*=1.77f;
    Ray ray;
    ray.origin = rayOrigin;
    ray.direction = rayDir;
    float destroyR = pow(float(scroll)/100,2);
    
    bvec3 mask;
    ivec3 map = ivec3(floor(ray.origin+0.f));
    vec3 deltaDist = abs(1/ray.direction);
    ivec3 _step = ivec3(sign(rayDir));
    vec3 sideDist = (sign(ray.direction) * (vec3(map)-ray.origin) + (sign(ray.direction)*0.5)+0.5)*deltaDist;

    bool found = false;

    // branchless DDA from www.shadertoy.com/view/4dX3zl
    for(int i = 0; i< 1000; i++){
        mask = lessThanEqual(sideDist.xyz, min(sideDist.yzx, sideDist.zxy));
        sideDist += vec3(mask) * deltaDist;
	    map += ivec3(vec3(mask)) * _step;
        
        if(getVoxel(map)){
            found = true;
            break;
        }
    }

    // applying colors
    vec3 color = vec3(0.6f,0.7f,0.9f);
    color += 0.4*rayDir.y;

    if(found){
        if(mouse == 1 && dot(pos,pos)<destroyR){
            S[number*number*map.z+number*map.y+map.x] = 0;
        }
        if(mouse == 2 && dot(pos,pos)<destroyR){
            S[number*number*(map.z-int(round(rayDir.z)))+number*(map.y-int(round(rayDir.y)))+(map.x-int(round(rayDir.x)))] = 1;
        }
        if (mask.x) {
            
		    color = vec3(max(0,dot(vec3(1,0,0),normalize(lightSource-map))));
            color -= 0.8*rayDir.y;
	    }
	    if (mask.y) {

		    color = vec3(max(0,dot(vec3(0,1,0),normalize(lightSource-map))));
            color -= 1*rayDir.y;
	    }
	    if (mask.z) {

		    color = vec3(max(0,dot(vec3(0,0,1),normalize(lightSource-map))));;
            color -= 0.8*rayDir.y;
	    }
        
    }
    
    // output pixel frangment
    FragColor = vec4(color, 1.0f);
}