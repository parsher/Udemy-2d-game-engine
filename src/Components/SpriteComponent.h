#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component {
  private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    bool isAnimated;
    int numFrames;
    int animationSpeed;
    bool isFixed;
    std::map<std::string, Animation> animations;
    std::string currentAnimationName;
    unsigned int animationIndex = 0;

  public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(const char* filePath) {
      isAnimated = false;
      isFixed = false;
      SetTexture(filePath);
    }

    SpriteComponent(std::string id, int numFrames, int animationSpeed, bool hasDirectins, bool isFixed) {
      isAnimated = true;
      this->numFrames;
      this->animationSpeed = animationSpeed;
      this->isFixed = isFixed;

      if (hasDirectins) {
        Animation downAnimation = Animation(0, numFrames, animationSpeed);
        Animation rightAnimation = Animation(1, numFrames, animationSpeed);
        Animation leftAnimation = Animation(2, numFrames, animationSpeed);
        Animation upAnimation = Animation(3, numFrames, animationSpeed);

        animations.emplace("DownAnimation", downAnimation);
        animations.emplace("RightAnimation", rightAnimation);
        animations.emplace("LeftAnimation", leftAnimation);
        animations.emplace("UpAnimation", upAnimation);

        this->animationIndex = 0;
        this->currentAnimationName = "DownAnimation";

      } else {
        Animation singleAnimation = Animation(0, numFrames, animationSpeed);
        animations.emplace("SingleAnimation", singleAnimation);
        this->animationIndex = 0;
        this->currentAnimationName = "SingleAnimation";
      }

      Play(this->currentAnimationName);
      SetTexture(id);

    }

    void Play(std::string animationName) {
      numFrames = animations[animationName].numFrames;
      animationIndex = animations[animationName].index;
      animationSpeed = animations[animationName].animationSpeed;
      currentAnimationName = animationName;
    }

    void SetTexture(std::string assetTextureId) {
      texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void Initialize() override {
      transform = owner->GetComponent<TransformComponent>();
      sourceRectangle.x = 0;
      sourceRectangle.y = 0;
      sourceRectangle.w = transform->width;
      sourceRectangle.h = transform->height;
    }

    void Update(float deltaTime) override {
      if (isAnimated) {
        sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
      }
      sourceRectangle.y = animationIndex * transform->height;

      destinationRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
      destinationRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
      destinationRectangle.w = transform->width * transform->scale;
      destinationRectangle.h = transform->height * transform->scale;
    }

    void Render() override {
      TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
    }
};

#endif