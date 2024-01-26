#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <optional>
#include <random>

template<typename ObservationSpaceT, typename ActionSpaceT>
struct transition final
{
  using observation_space_type = ObservationSpaceT;
  using observation_type       = observation_space_type::type;
  using action_space_type      = ActionSpaceT;
  using action_type            = action_space_type::type;

  transition(const transition &)            = default;
  transition(transition &&)                 = default;
  transition &operator=(const transition &) = default;
  transition &operator=(transition &&)      = default;

  transition(
      const observation_type &observation,
      const action_type      &action,
      const observation_type &next_observation,
      double                  reward,
      bool                    terminated,
      bool                    truncated
  )
      : observation(observation)
      , action(action)
      , next_observation(next_observation)
      , reward(reward)
      , terminated(terminated)
      , truncated(truncated)
  {
  }

  observation_type observation;
  action_type      action;
  observation_type next_observation;
  double           reward;
  bool             terminated;
  bool             truncated;
};

template<typename ObservationSpaceT, typename ActionSpaceT>
class basic_environment
{
public:
  using observation_space_type = ObservationSpaceT;
  using observation_type       = observation_space_type::type;
  using action_space_type      = ActionSpaceT;
  using action_type            = action_space_type::type;
  using environment_type       = basic_environment<observation_space_type, action_space_type>;
  using transition_type        = transition<observation_space_type, action_space_type>;

  basic_environment(const basic_environment &)            = delete;
  basic_environment(basic_environment &&)                 = delete;
  basic_environment &operator=(const basic_environment &) = delete;
  basic_environment &operator=(basic_environment &&)      = delete;

  basic_environment(const observation_space_type &observation_space, const action_space_type &action_space)
      : _observation_space(observation_space)
      , _action_space(action_space)
      , _random()
  {
  }

  virtual ~basic_environment() {}

  observation_space_type observation_space() { return _observation_space; }

  observation_type observation() { return _observation; }

  action_space_type action_space() { return _action_space; }

  observation_type reset(std::optional<std::uint32_t> seed = std::nullopt)
  {
    _random.seed(seed.value_or(std::random_device()()));
    return _observation = do_reset();
  }

  transition_type step(action_type action)
  {
    auto observation = _observation;
    auto transition  = do_step(observation, action);
    _observation     = transition.next_observation;
    return transition;
  }

  std::default_random_engine &random() { return _random; }

protected:
  virtual observation_type do_reset()                                                = 0;
  virtual transition_type  do_step(observation_type observation, action_type action) = 0;

private:
  observation_space_type     _observation_space;
  observation_type           _observation;
  action_space_type          _action_space;
  std::default_random_engine _random;
};

template<typename T>
struct space
{
  using type = T;
};

template<typename T>
struct box final : space<T>
{
  constexpr box(const T &low, const T &hight)
      : low(low)
      , hight(hight)
  {
  }

  T low;
  T hight;
};

struct discrete final : space<std::int32_t>
{
  constexpr explicit discrete(std::int32_t size)
      : size(size)
  {
  }

  std::int32_t size;
};

class cart_pole_environment final : public basic_environment<box<std::array<float, 4>>, discrete>
{
  static constexpr float x_threshold     = 2.4f;
  static constexpr float theta_threshold = 12.f * 2.f * std::numbers::pi_v<float> / 360.f;
  static constexpr float lowest          = std::numeric_limits<float>::lowest();
  static constexpr float highest         = std::numeric_limits<float>::max();

  static constexpr action_type right = 1;

  static constexpr float gravity          = 9.8f;
  static constexpr float mass_cart        = 1.0f;
  static constexpr float mass_pole        = 0.1f;
  static constexpr float total_mass       = mass_cart + mass_pole;
  static constexpr float length           = 0.5f;
  static constexpr float pole_mass_length = mass_pole * length;
  static constexpr float force_mag        = 10.0f;
  static constexpr float tau              = 0.02f;

public:
  cart_pole_environment(const cart_pole_environment &)            = delete;
  cart_pole_environment(cart_pole_environment &&)                 = delete;
  cart_pole_environment &operator=(const cart_pole_environment &) = delete;
  cart_pole_environment &operator=(cart_pole_environment &&)      = delete;

  cart_pole_environment()
      : environment_type(
          observation_space_type(
              {-x_threshold * 2, lowest, -theta_threshold * 2, lowest},
              {x_threshold * 2, highest, theta_threshold * 2, highest}
          ),
          action_space_type(2)
      )
  {
  }

protected:
  observation_type do_reset() override
  {
    _steps_beyond_terminated.reset();
    std::uniform_real<float> distribution(-0.05f, 0.05f);
    return {distribution(random()), distribution(random()), distribution(random()), distribution(random())};
  }

  transition_type do_step(observation_type observation, action_type action) override
  {
    auto [x, x_v, theta, theta_v] = observation;
    auto force                    = action == right ? force_mag : -force_mag;
    auto sin_theta                = std::sin(theta);
    auto cos_theta                = std::cos(theta);

    auto temp      = (force + pole_mass_length * std::pow(theta_v, 2) * sin_theta) / total_mass;
    auto theta_acc = (gravity * sin_theta - cos_theta * temp)
                     / (length * (4.0f / 3.0f - mass_pole * std::pow(cos_theta, 2) / total_mass));
    auto x_acc = temp - pole_mass_length * theta_acc * cos_theta / total_mass;

    x       += tau * x_v;
    x_v     += tau * x_acc;
    theta   += tau * theta_v;
    theta_v += tau * theta_acc;

    observation_type next_observation = {x, x_v, theta, theta_v};
    auto terminated = x < -x_threshold or x > x_threshold or theta < -theta_threshold or theta > theta_threshold;
    if (!terminated)
      return {observation, action, next_observation, 1.0f, false, false};

    if (not _steps_beyond_terminated.has_value())
    {
      _steps_beyond_terminated.emplace(0);
      return {observation, action, next_observation, 1.0f, true, false};
    }

    _steps_beyond_terminated.emplace(_steps_beyond_terminated.value() + 1);
    return {observation, action, next_observation, 0.0f, true, false};
  }

private:
  std::optional<std::int32_t> _steps_beyond_terminated;
};

int main()
{
  cart_pole_environment environment;
  environment.reset();
  auto transition = environment.step(0);
  return 0;
}
