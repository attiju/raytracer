#include "transform.hpp"
#include "ray.hpp"
#include "sphere.hpp"

#include "image.hpp"
#include "interaction.hpp"
#include "camera.hpp"
#include "film.hpp"

int main()
{
    auto c2w    = lookAt({ 0, 0, 0 }, { 0, 0, -50 });
    auto film    = Film({ 800, 600 }, 35);
    auto camera = Camera(c2w, &film, 0, 0, 55);

    auto o2w    = translate({0, 5, -50});
    auto w2o    = inverse(o2w);
    auto sphere = Sphere(&o2w, &w2o, 5);

    auto l = Point3f(25, 50, 0);

    //generate some image
    unsigned width = 600, height = 800;
    Image    image(width, height);

    SurfaceInteraction intr;

    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            auto r = camera.next_ray(CameraSample({ Float(x), Float(y) }, { }));

            //std::cout << r << std::endl;

            unsigned char c0 = 0;
            unsigned char c1 = 0;
            unsigned char c2 = 0;

            if (sphere.intersects(r, &intr)) {
                auto fact = 255.5 * max(0., dot(Vector3f(intr.n), normalize(l - intr.p)));

                c0 = static_cast<unsigned char>((1 + intr.n.x) * 0.5 * fact);
                c1 = static_cast<unsigned char>((1 + intr.n.y) * 0.5 * fact);
                c2 = static_cast<unsigned char>((1 + intr.n.z) * 0.5 * fact);
            }

            *image.at(x, y, 0) = c0;
            *image.at(x, y, 1) = c1;
            *image.at(x, y, 2) = c2;
        }
    }

    image.encode("test.png");

    return 0;
}
