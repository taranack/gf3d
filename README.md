# Volley Heroes
A demo game and engine built on Vulkan and SDL2. This project was built off the forked framework GF3D. The initial framework sets up the rendering pipeline, while my work implements systems such as entities, animations, and particle effects.


# Build Process

1. Obtain the code: `git clone <repo name>`
2. Checkout seed branch: `git checkout <branch name>`
3. Make sure ou fetch submodules: `git submodule update --init --recursive`
4. Build libraries: `pushd gfc/src; make; popd`
5. Build game: `pushd src; make; popd`

You should now have a `gf3d` binary within the root of your git repository. Executing this will start your game.
