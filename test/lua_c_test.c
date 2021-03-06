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
                                 printf("%s", lua_tostring(L, i));
                                 break;
                             }
            case LUA_TBOOLEAN:{
                                  printf(lua_toboolean(L, i)?"true":"false");
                                  break;
                              }
            case LUA_TNUMBER:{
                                 if(lua_isinteger(L, i)){
                                     printf("%lld", lua_tointeger(L, i));
                                 }
                                 else{
                                    printf("%g", lua_tonumber(L, i));
                                 }
                                 break;
                             }
            default:
                             {
                                 printf("%s", lua_typename(L, t));
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
    char buff[256];
    int error;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    while(fgets(buff, sizeof(buff), stdin) != NULL){
        error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
            lua_pcall(L, 0, 0, 0);
        stackDump(L);
        if (error) {
            fprintf(stderr, "%s", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    lua_close(L);
    return 0;
}
