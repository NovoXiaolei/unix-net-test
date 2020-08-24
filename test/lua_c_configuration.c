#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_COLOR 255

static void
stackDump(lua_State *L){
    int i;
    int top = lua_gettop(L);//获得stack的深度
    for (i = 1; i<= top; i++){
        int t = lua_type(L, i); //用于返回栈中玄素的类型
        switch(t){
            case LUA_TSTRING:{
                                 printf("%d %s", top - i + 1, lua_tostring(L, i));
                                 break;
                             }
            case LUA_TBOOLEAN:{
                                  printf("%d, %s", top - i + 1, lua_toboolean(L, i)?"true":"false");
                                  break;
                              }
            case LUA_TNUMBER:{
                                 if(lua_isinteger(L, i)){
                                     printf("%d %lld", top - i + 1, lua_tointeger(L, i));
                                 }
                                 else{
                                    printf("%d %g", top - i + 1,lua_tonumber(L, i));
                                 }
                                 break;
                             }
            default:
                             {
                                 printf(" %d %s", top - i + 1, lua_typename(L, t));
                                 break;
                             }

        }
        printf(" : ");
    }
    printf("\n");
}


void
error(lua_State *L, const char *fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

int
getcolorfield(lua_State *L, const char *key){
    int result, isnum;
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    result = (int)(lua_tonumberx(L, -1, &isnum) * MAX_COLOR);
    if(!isnum)
        error(L, "invalid component '%s', is color", key);
    lua_pop(L, 1);
    return result;
}

int
getglobalint(lua_State *L, const char *var){
    int isnum, result;
    lua_getglobal(L, var);
    result = (int)lua_tointegerx(L, -1, &isnum);
    if (!isnum)
        error(L, "'%s' should be a number\n", var);
    lua_pop(L, 1);
    return result;
}

void
load(lua_State *L, const char *fname, int *w, int *h){
    if(luaL_loadfile(L, fname) ||
            lua_pcall(L, 0, 0, 0))
        error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    *w = getglobalint(L, "width");
    *h = getglobalint(L, "height");
}

double
f(lua_State *L, double x, double y){
    int isnum;
    double z;
    lua_getglobal(L, "f");
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    if (lua_pcall(L, 2, 1, 0) != LUA_OK)
        error(L, "error running function 'f':%s",
                lua_tostring(L, -1));
    z = lua_tonumberx(L, -1, &isnum);
    if(!isnum)
        error(L, "function 'f' should return a number");
    lua_pop(L, 1);//弹出返回值
    return z;
}

int
main(void){
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    int w = 0;
    int h = 0;
    load(L, "config.lua", &w, &h);
    printf("width = %d, height = %d\n", w, h);

    lua_getglobal(L, "background");
    if(!lua_istable(L, -1))
        error(L, "background is not a table");
    int red = getcolorfield(L, "red");
    int green = getcolorfield(L, "green");
    int blue = getcolorfield(L, "blue");
    printf(" r = %d, g = %d, b = %d\n", red, green, blue);

    int result = f(L, 10, 10);
    printf("f result = %d\n", result);

    lua_close(L);
    return 0;
}
