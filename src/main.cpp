#include "cpu.h"
#include "io.h"

static void print_usage(char** argv)
{
    std::cerr << "usage: " << argv[0] << " [--test] [--image FILE] [--blk FILE]" << std::endl;
}

int main(int argc, char** argv)
{
    typedef std::pair<std::string, std::optional<std::string>> Arg;
    std::array<Arg, 3> args = {{
        { "--test",         "n" },
        { "--image",        std::nullopt },
        { "--blk",          std::nullopt }
    }};

    // Parse argc
    int valid_skip = -1;
    for (int i = 1; i < argc; i++)
    {
        bool found = false;
        for (size_t j = 0; j < args.size(); ++j)
        {
            if (std::string(argv[i]) == args[j].first)
            {
                if (args[j].first == "--test")
                    args[j].second = "y";
                else
                {
                    args[j].second = std::string(argv[i + 1]);
                    valid_skip = i + 1;
                }

                found = true;
            }
        }

        if (!found && i != valid_skip)
        {
            std::cerr << "unknown argument: " << argv[i] << std::endl;
            print_usage(argv);
            return 1;
        }
    }
    const bool test_mode = args[0].second == "y";

    // Validate args
    if (!args[1].second.has_value())
    {
        std::cerr << "no image file specified" << std::endl;
        print_usage(argv);
        return 1;
    }

    // Set up CPU
    CPU cpu(
        test_mode ? (16 * 1024 * 1024) : (2UL * 1024 * 1024 * 1024),
        test_mode,
        args[2].second
    );

    // Load main program / image
    std::ignore = cpu.bus.write_file(Bus::programs_base, *args[1].second);

    // Enter emulation loop
    while(1)
    {
        if (test_mode)
            cpu.trace();

        cpu.do_cycle();
        cpu.bus.clock(cpu);

        const std::optional<CPU::PendingTrap> trap = cpu.get_pending_trap();
        if (trap.has_value())
            cpu.handle_trap(trap->cause, trap->info, trap->is_interrupt);
    }
}
