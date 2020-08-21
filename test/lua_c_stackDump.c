#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

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
main(void){
    lua_State *L = luaL_newstate();
    lua_pushboolean(L,1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "hello");

    stackDump(L);

    lua_pushvalue(L, -4);stackDump(L);


    lua_replace(L,3);stackDump(L);
    lua_settop(L, 6);stackDump(L);

    //从3号索引向栈顶一定一个位置
    lua_rotate(L, 3, 1);stackDump(L);


    lua_close(L);
    return 0;
}
