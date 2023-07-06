#include "include/ImageDisplayer.h"
#include "include/TextureManager.h"

ImageDisplayer::ImageDisplayer(const char* imagepath, int w, int h) {
    rect = {0, 0, w, h};
    img = nullptr;
    setImage(imagepath);
}

ImageDisplayer::~ImageDisplayer() {}

void ImageDisplayer::setImage(const char* imagepath) {
    destroy();
    //img = TextureManager::LoadTexture(imagepath);
}

void ImageDisplayer::draw() {
    TextureManager::DrawFilledRect(&rect, "white");
    //TextureManager::Draw(img, nullptr, &rect);
    TextureManager::DrawRect(&rect, "black");
}

void ImageDisplayer::destroy() {
    if (img == nullptr) return;
    SDL_DestroyTexture(img);
    img = nullptr;
}
