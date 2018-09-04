#include <string_view>
#include <array>
#include <memory>

class native_script {
    public:
    virtual ~native_script() = default;
    virtual const std::string_view run() const noexcept = 0;
};

class script_torch : public native_script {
    public:
    const std::string_view run() const noexcept override { return "I'm a torch"; }
};

class script_candle : public native_script {
    public:
    const std::string_view run() const noexcept override { return "I'm a candle"; }
};

using script_ptr = const std::unique_ptr<native_script>;
using script_factory = auto (*)() -> script_ptr;

int main() {
    constexpr script_factory factories[] {
        []() -> const script_ptr { return std::make_unique<script_torch>(); },
        []() -> const script_ptr { return std::make_unique<script_candle>(); }
    };

    const auto torch = factories[0]();
    const auto candle = factories[1]();

    //torch->run();
    return candle->run()[0];
}