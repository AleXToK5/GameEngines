#include "AsteroidSpawnerSystem.h"
#include "../../Core/SpawnerConfigComponent.h"

AsteroidSpawnerSystem::AsteroidSpawnerSystem(World &world, ConfigReader &config)
    : ISystem(world),
      _config(config),
      _positions(world.GetStorage<PositionComponent>()),
      _movements(world.GetStorage<MovementComponent>()),
      _renders(world.GetStorage<RenderComponent>()),
      _colliders(world.GetStorage<ColliderComponent>()),
      _asteroids(world.GetStorage<AsteroidComponent>()) {
    // Инициализируем генератор случайных чисел
    std::random_device rd;
    _rng = std::mt19937(rd());
}

float AsteroidSpawnerSystem::GetRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(_rng);
}

int AsteroidSpawnerSystem::GetRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(_rng);
}

void AsteroidSpawnerSystem::ResetTimer() {
    // Берем миллисекунды из конфига, выбираем случайное значение и переводим в кадры (60 FPS)
    int minMs = _config.GetAsteroidSpawnTimeMinMs();
    int maxMs = _config.GetAsteroidSpawnTimeMaxMs();
    int randomMs = GetRandomInt(minMs, maxMs);

    _framesUntilNextSpawn = (randomMs * 60) / 1000;
}

void AsteroidSpawnerSystem::OnInit() {
    ResetTimer();
}

void AsteroidSpawnerSystem::OnUpdate() {
    // Получаем динамический конфиг из ECS (из ImGui)
    SpawnerConfigComponent *liveCfg = nullptr;
    for (int ent: world.GetStorage<SpawnerConfigComponent>().Entities()) {
        liveCfg = &world.GetStorage<SpawnerConfigComponent>().Get(ent);
        break;
    }

    _framesUntilNextSpawn--;

    if (_framesUntilNextSpawn <= 0 || (liveCfg && liveCfg->ForceSpawn)) {
        int asteroid = world.CreateEntity();

        float size = GetRandomFloat(_config.GetAsteroidSizeMin(), _config.GetAsteroidSizeMax());
        float speed = GetRandomFloat(_config.GetAsteroidSpeedMin(), _config.GetAsteroidSpeedMax()) / 60.0f;

        float dirX = GetRandomFloat(_config.GetAsteroidDirectionXMin(), _config.GetAsteroidDirectionXMax());
        float dirY = GetRandomFloat(_config.GetAsteroidDirectionYMin(), _config.GetAsteroidDirectionYMax());

        float length = std::sqrt(dirX * dirX + dirY * dirY);
        if (length != 0) {
            dirX /= length;
            dirY /= length;
        } else {
            dirY = 1.0f;
        }

        float startX = GetRandomFloat(0.f, (float) _config.GetWindowWidth());
        float startY = -size - 10.f;

        _positions.Add(asteroid, PositionComponent{startX, startY});
        _movements.Add(asteroid, MovementComponent{speed, sf::Vector2f(dirX, dirY)});

        int points = GetRandomInt(_config.GetAsteroidCirclePointsMin(), _config.GetAsteroidCirclePointsMax());
        _renders.Add(asteroid, RenderComponent{ShapeType::Polygon, size, points, {0.f, 0.f}, sf::Color::Red});
        _colliders.Add(asteroid, ColliderComponent{
                           ColliderType::Circle, size, {0.f, 0.f}, Asteroid, static_cast<uint16_t>(Player | Projectile)
                       });
        _asteroids.Add(asteroid, AsteroidComponent{});

        if (liveCfg) {
            int randomMs = GetRandomInt(liveCfg->MinSpawnTimeMs, liveCfg->MaxSpawnTimeMs);
            _framesUntilNextSpawn = (randomMs * 60) / 1000;
            liveCfg->ForceSpawn = false;
        } else {
            ResetTimer();
        }
    }
}
