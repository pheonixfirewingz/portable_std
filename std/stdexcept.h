#ifndef STDEXCEPT_H
#define STDEXCEPT_H
namespace std
{
    class [[nodiscard]] exception
    {
    public:
        exception() noexcept = default;
        exception(const exception&) noexcept = default;
        exception(exception&&) noexcept = default;
        virtual ~exception() = default;
        exception& operator=(const exception&) noexcept = default;
        exception& operator=(exception&&) noexcept = default;

        [[nodiscard]] virtual const char* what() const noexcept {
            return "std::exception";
        }
    };

    class [[nodiscard]] logic_error : public exception {
    public:
        using exception::exception;
        logic_error(const logic_error&) noexcept = default;
        logic_error(logic_error&&) noexcept = default;
        logic_error& operator=(const logic_error&) noexcept = default;
        logic_error& operator=(logic_error&&) noexcept = default;
        [[nodiscard]] const char* what() const noexcept override {
            return "logic error";
        }
    };

    class [[nodiscard]] runtime_error : public exception {
    public:
        using exception::exception;
        runtime_error(const runtime_error&) noexcept = default;
        runtime_error(runtime_error&&) noexcept = default;
        runtime_error& operator=(const runtime_error&) noexcept = default;
        runtime_error& operator=(runtime_error&&) noexcept = default;
        explicit runtime_error(const char* what) noexcept : _what(what) {}
        
        [[nodiscard]] virtual const char* what() const noexcept {
            return _what;
        }
        private:
         const char* _what = "std::exception";
    };

    class [[nodiscard]] range_error : public logic_error {
    public:
        using logic_error::logic_error;
        range_error(const range_error&) noexcept = default;
        range_error(range_error&&) noexcept = default;
        range_error& operator=(const range_error&) noexcept = default;
        range_error& operator=(range_error&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept override {
            return "range error";
        }
    };

    class [[nodiscard]] domain_error final : public runtime_error {
    public:
        using runtime_error::runtime_error;
        domain_error(const domain_error&) noexcept = default;
        domain_error(domain_error&&) noexcept = default;
        domain_error& operator=(const domain_error&) noexcept = default;
        domain_error& operator=(domain_error&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept final override {
            return "domain error";
        }
    };

    class [[nodiscard]] length_error final : public logic_error {
    public:
        using logic_error::logic_error;
        length_error(const length_error&) noexcept = default;
        length_error(length_error&&) noexcept = default;
        length_error& operator=(const length_error&) noexcept = default;
        length_error& operator=(length_error&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept final override {
            return "length error";
        }
    };

    class [[nodiscard]] out_of_range final : public range_error {
    public:
        using range_error::range_error;
        out_of_range(const out_of_range&) noexcept = default;
        out_of_range(out_of_range&&) noexcept = default;
        out_of_range& operator=(const out_of_range&) noexcept = default;
        out_of_range& operator=(out_of_range&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept final override {
            return "out of range";
        }
    };

    class [[nodiscard]] overflow_error final : public runtime_error {
    public:
        using runtime_error::runtime_error;
        overflow_error(const overflow_error&) noexcept = default;
        overflow_error(overflow_error&&) noexcept = default;
        overflow_error& operator=(const overflow_error&) noexcept = default;
        overflow_error& operator=(overflow_error&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept final override {
            return "overflow error";
        }
    };

    class [[nodiscard]] underflow_error : public runtime_error {
    public:
        using runtime_error::runtime_error;
        underflow_error(const underflow_error&) noexcept = default;
        underflow_error(underflow_error&&) noexcept = default;
        underflow_error& operator=(const underflow_error&) noexcept = default;
        underflow_error& operator=(underflow_error&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept final override {
            return "underflow error";
        }
    };

    class [[nodiscard]] invalid_argument final : public logic_error {
    public:
        using logic_error::logic_error;
        invalid_argument(const invalid_argument&) noexcept = default;
        invalid_argument(invalid_argument&&) noexcept = default;
        invalid_argument& operator=(const invalid_argument&) noexcept = default;
        invalid_argument& operator=(invalid_argument&&) noexcept = default;

        [[nodiscard]] const char* what() const noexcept final override {
            return "invalid argument";
        }
    };
}
#endif