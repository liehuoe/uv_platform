#include <tclap.hpp>

class Args {
  private:
    static Args args; // 在 main.cpp 定义
  public:
    /** 姓名 */
    TCLAP::ValueArg<std::string> name{"n", "name", "Name to print", false, "homer", "string"};
    /** 是否反转 */
    TCLAP::SwitchArg reverse{"r", "reverse", "Print name backwards", false};

  public:
    static Args &instance() { return Args::args; }
    Args &init(int argc, char **argv) {
        try {
            TCLAP::CmdLine cmd("example program, show the basic usage");
            cmd.add(name).add(reverse);
            cmd.parse(argc, argv);

            // std::string n = name.getValue();
            // bool reverseName = reverse.getValue();

            // if (reverseName) {
            //     std::reverse(n.begin(), n.end());
            //     std::cout << "My name (spelled backwards) is: " << n << std::endl;
            // } else
            //     std::cout << "My name is: " << n << std::endl;

        } catch (TCLAP::ArgException &e) {
            std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        }
        return *this;
    }
};
