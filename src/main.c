#include <SDL2/SDL.h>
#include <wz/file.h>

wz2d *
wz_open_origin(wzvar * component) {
  wzvar * origin = wz_open_var(component, "origin");
  wz2d * vec = &wz_get_vec(origin)->val;
  wzimg * img = wz_get_img(component);
  return vec->x = (int32_t) (img->w - (uint32_t) vec->x), vec;
}

wz2d *
wz_open_map(wzvar * component, const char * name) {
  wzvar * map = wz_open_var(component, "map");
  wzvar * var = wz_open_var(map, name);
  wz2d * vec = &wz_get_vec(var)->val;
  return vec->x = - vec->x, vec;
}

SDL_Texture *
wz_img_to_tex(wzimg * img, SDL_Renderer * renderer) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  Uint32 rmask = 0x0000ff00;
  Uint32 gmask = 0x00ff0000;
  Uint32 bmask = 0xff000000;
  Uint32 amask = 0x000000ff;
#else
  Uint32 rmask = 0x00ff0000;
  Uint32 gmask = 0x0000ff00;
  Uint32 bmask = 0x000000ff;
  Uint32 amask = 0xff000000;
#endif
  SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(
      img->data, (int) img->w, (int) img->h, 32, (int) img->w * 4,
      rmask, gmask, bmask, amask);
  if (surface == NULL) return NULL;
  SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (texture == NULL) return NULL;
  return texture;
}

int
main(void) {
  int ret = 1;

  // wz
  wzctx ctx;
  if (wz_init_ctx(&ctx)) goto wz_init_ctx_fail;
  wzfile file;
  if (wz_open_file(&file, "../Character.wz", &ctx)) goto wz_open_file_fail;
  wznode * node_root = wz_open_root_node(&file);
  wznode * node_body = wz_open_node(node_root, "00002000.img");
  wznode * node_head = wz_open_node(node_root, "00012000.img");
  wznode * node_face = wz_open_node(node_root, "Face/00020000.img");
  wznode * node_hair = wz_open_node(node_root, "Hair/00030020.img");

  wzvar * root_body = wz_open_root_var(node_body);
  wzvar * root_head = wz_open_root_var(node_head);
  wzvar * root_face = wz_open_root_var(node_face);
  wzvar * root_hair = wz_open_root_var(node_hair);

  wzvar * body = wz_open_var(root_body, "walk1/0/body");
  wz2d * body_to_navel = wz_open_map(body, "navel");
  wz2d * body_to_neck = wz_open_map(body, "neck");
  wz2d * body_origin = wz_open_origin(body);
  wzimg * body_img = wz_get_img(body);
  int32_t navel_x = body_to_navel->x;
  int32_t navel_y = body_to_navel->y;
  int32_t neck_x = body_to_neck->x;
  int32_t neck_y = body_to_neck->y;

  wzvar * arm = wz_open_var(root_body, "walk1/0/arm");
  wz2d * arm_to_navel = wz_open_map(arm, "navel");
  wz2d * arm_origin = wz_open_origin(arm);
  wzimg * arm_img = wz_get_img(arm);
  int32_t arm_x = navel_x - arm_to_navel->x;
  int32_t arm_y = navel_y - arm_to_navel->y;

  wzvar * head = wz_open_var(root_head, "front/head");
  wz2d * head_to_neck = wz_open_map(head, "neck");
  wz2d * head_to_brow = wz_open_map(head, "brow");
  wz2d * head_origin = wz_open_origin(head);
  wzimg * head_img = wz_get_img(head);
  int32_t head_x = neck_x - head_to_neck->x;
  int32_t head_y = neck_y - head_to_neck->y;
  int32_t brow_x = head_x + head_to_brow->x;
  int32_t brow_y = head_y + head_to_brow->y;

  wzvar * face = wz_open_var(root_face, "default/face");
  wz2d * face_to_brow = wz_open_map(face, "brow");
  wz2d * face_origin = wz_open_origin(face);
  wzimg * face_img = wz_get_img(face);
  int32_t face_x = brow_x - face_to_brow->x;
  int32_t face_y = brow_y - face_to_brow->y;

  wzvar * hair_shade = wz_open_var(root_hair, "default/hairShade/0");
  wz2d * hair_shade_to_brow = wz_open_map(hair_shade, "brow");
  wz2d * hair_shade_origin = wz_open_origin(hair_shade);
  wzimg * hair_shade_img = wz_get_img(hair_shade);
  int32_t hair_shade_x = brow_x - hair_shade_to_brow->x;
  int32_t hair_shade_y = brow_y - hair_shade_to_brow->y;

  wzvar * hair = wz_open_var(root_hair, "default/hair");
  wz2d * hair_to_brow = wz_open_map(hair, "brow");
  wz2d * hair_origin = wz_open_origin(hair);
  wzimg * hair_img = wz_get_img(hair);
  int32_t hair_x = brow_x - hair_to_brow->x;
  int32_t hair_y = brow_y - hair_to_brow->y;

  wzvar * hair_over_head = wz_open_var(root_hair, "default/hairOverHead");
  wz2d * hair_over_head_to_brow = wz_open_map(hair_over_head, "brow");
  wz2d * hair_over_head_origin = wz_open_origin(hair_over_head);
  wzimg * hair_over_head_img = wz_get_img(hair_over_head);
  int32_t hair_over_head_x = brow_x - hair_over_head_to_brow->x;
  int32_t hair_over_head_y = brow_y - hair_over_head_to_brow->y;

  wzvar * hair_below_body = wz_open_var(root_hair, "default/hairBelowBody");
  wz2d * hair_below_body_to_brow = wz_open_map(hair_below_body, "brow");
  wz2d * hair_below_body_origin = wz_open_origin(hair_below_body);
  wzimg * hair_below_body_img = wz_get_img(hair_below_body);
  int32_t hair_below_body_x = brow_x - hair_below_body_to_brow->x;
  int32_t hair_below_body_y = brow_y - hair_below_body_to_brow->y;

  // SDL
  if (SDL_Init(SDL_INIT_VIDEO)) goto SDL_Init_fail;
  SDL_Window * window = SDL_CreateWindow(
      "WzWin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 620,
      SDL_WINDOW_SHOWN);
  if (window == NULL) goto SDL_CreateWindow_fail;
  SDL_Renderer * renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) goto SDL_CreateRenderer_fail;
  SDL_SetRenderDrawColor(renderer, 0xcc, 0xcc, 0xcc, 0xff);
  SDL_Texture * texture_body = wz_img_to_tex(body_img, renderer);
  if (texture_body == NULL) goto wz_img_to_tex_fail;
  SDL_Texture * texture_arm = wz_img_to_tex(arm_img, renderer);
  if (texture_arm == NULL) goto wz_img_to_tex_fail2;
  SDL_Texture * texture_head = wz_img_to_tex(head_img, renderer);
  if (texture_head == NULL) goto wz_img_to_tex_fail3;
  SDL_Texture * texture_face = wz_img_to_tex(face_img, renderer);
  if (texture_face == NULL) goto wz_img_to_tex_fail4;
  SDL_Texture * texture_hair = wz_img_to_tex(hair_img, renderer);
  if (texture_hair == NULL) goto wz_img_to_tex_fail5;
  SDL_Texture * texture_hair_shade = wz_img_to_tex(hair_shade_img, renderer);
  if (texture_hair_shade == NULL) goto wz_img_to_tex_fail6;
  SDL_Texture * texture_hair_over_head =
      wz_img_to_tex(hair_over_head_img, renderer);
  if (texture_hair_over_head == NULL) goto wz_img_to_tex_fail7;
  SDL_Texture * texture_hair_below_body =
      wz_img_to_tex(hair_below_body_img, renderer);
  if (texture_hair_below_body == NULL) goto wz_img_to_tex_fail8;
  for (int quit = 0; !quit;) {
    SDL_Event e;
    if (!SDL_WaitEvent(&e)) continue;
    switch (e.type) {
    case SDL_QUIT: quit = 1; break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym) {
      case SDLK_UP: printf("up.\n"); break;
      case SDLK_DOWN: printf("down.\n"); break;
      case SDLK_LEFT: printf("left.\n"); break;
      case SDLK_RIGHT: printf("right.\n"); break;
      default: printf("default.\n"); break;
      }
    }
    SDL_RenderClear(renderer);
    int rate = 8;
    SDL_Rect origin;
    origin.x = 25;
    origin.y = 40;

    SDL_Rect dst;

    dst.x = (int) (origin.x + body_origin->x + hair_below_body_x -
                   hair_below_body_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + hair_below_body_y -
                   hair_below_body_origin->y) * rate;
    dst.w = (int) hair_below_body_img->w * rate;
    dst.h = (int) hair_below_body_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_hair_below_body, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) origin.x * rate;
    dst.y = (int) origin.y * rate;
    dst.w = (int) body_img->w * rate;
    dst.h = (int) body_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_body, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) (origin.x + body_origin->x + arm_x - arm_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + arm_y - arm_origin->y) * rate;
    dst.w = (int) arm_img->w * rate;
    dst.h = (int) arm_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_arm, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) (origin.x + body_origin->x + head_x - head_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + head_y - head_origin->y) * rate;
    dst.w = (int) head_img->w * rate;
    dst.h = (int) head_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_head, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) (origin.x + body_origin->x + face_x - face_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + face_y - face_origin->y) * rate;
    dst.w = (int) face_img->w * rate;
    dst.h = (int) face_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_face, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) (origin.x + body_origin->x + hair_shade_x -
                   hair_shade_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + hair_shade_y -
                   hair_shade_origin->y) * rate;
    dst.w = (int) hair_shade_img->w * rate;
    dst.h = (int) hair_shade_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_hair_shade, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) (origin.x + body_origin->x + hair_x - hair_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + hair_y - hair_origin->y) * rate;
    dst.w = (int) hair_img->w * rate;
    dst.h = (int) hair_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_hair, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    dst.x = (int) (origin.x + body_origin->x + hair_over_head_x -
                   hair_over_head_origin->x) * rate;
    dst.y = (int) (origin.y + body_origin->y + hair_over_head_y -
                   hair_over_head_origin->y) * rate;
    dst.w = (int) hair_over_head_img->w * rate;
    dst.h = (int) hair_over_head_img->h * rate;
    SDL_RenderCopyEx(renderer, texture_hair_over_head, NULL, &dst, 0, NULL,
                     SDL_FLIP_HORIZONTAL);

    SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(texture_hair_below_body);
  ret = 0;
wz_img_to_tex_fail8: SDL_DestroyTexture(texture_hair_over_head);
wz_img_to_tex_fail7: SDL_DestroyTexture(texture_hair_shade);
wz_img_to_tex_fail6: SDL_DestroyTexture(texture_hair);
wz_img_to_tex_fail5: SDL_DestroyTexture(texture_face);
wz_img_to_tex_fail4: SDL_DestroyTexture(texture_head);
wz_img_to_tex_fail3: SDL_DestroyTexture(texture_arm);
wz_img_to_tex_fail2: SDL_DestroyTexture(texture_body);
wz_img_to_tex_fail: SDL_DestroyRenderer(renderer);
SDL_CreateRenderer_fail: SDL_DestroyWindow(window);
SDL_CreateWindow_fail: SDL_Quit();
SDL_Init_fail: wz_close_file(&file);
wz_open_file_fail: wz_free_ctx(&ctx);
wz_init_ctx_fail:
  return ret;
}
