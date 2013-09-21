#include <cstdio>
#include <cstdlib>
#include "tui.h"
#include "parser.h"

#ifndef ENTRYPOINT
# define ENTRYPOINT exe_main
#endif

#define NORETURNATTR __attribute__((__noreturn__))

int test(tui_t& tui)
{
    std::shared_ptr<AComponent> sp;

    tui.ln();
    sp.reset((new Label())->weight(1.0f)->text("Hello, world!"));
    tui.add(sp);
    sp.reset((new Button())->text("Close"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Button())->text("Okay"));
    tui.add(sp);
    sp.reset((new Button())->text("Cancel"));
    tui.add(sp);

    // with padding
    tui.ln();
    sp.reset((new Button())->text("Left"));
    tui.add(sp);
    sp.reset((new Label())->weight(1.0f));
    tui.add(sp);
    sp.reset((new Button())->text("Okay"));
    tui.add(sp);
    sp.reset((new Button())->text("Cancel"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Label())->weight(1.0f));
    tui.add(sp);

    tui.ln();
    sp.reset((new Button())->text("Btn"));
    tui.add(sp);
    sp.reset((new Combo())->weight(1.0f)->text("Type"));
    tui.add(sp);


    tui.ln();
    sp.reset((new Label())->weight(1.0f)->text("Line uno")->text("Line duo"));
    tui.add(sp);
    sp.reset((new Combo())->weight(0.5f)->text("Type"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Combo())->weight(0.5f)->text("Left 50%"));
    tui.add(sp);
    sp.reset((new Combo())->weight(0.5f)->text("Right 50%"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Label())->weight(1.0f)->text("Two blank lines after this:"));
    tui.add(sp);
    tui.ln();
    tui.ln();
    tui.ln();
    sp.reset((new Label())->weight(1.0f)->text("End two blank lines"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Label())->text("implicit weight"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Label())->weight(0.5f)->text("Text box"));
    tui.add(sp);
    sp.reset((new TextBox())->weight(0.8f)->text("Hello, world!"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Label())->text("Multi line text box below:"));
    tui.add(sp);
    tui.ln();
    sp.reset((new TextBox())->text("Line uno")->text("Line duo")->text("Line tres")->text("")->text("Line Quinto")->text("Lino ses"));
    tui.add(sp);

    tui.ln();
    sp.reset((new Label())->text("Multi line list box below:"));
    tui.add(sp);
    tui.ln();
    sp.reset((new ListBox())->text("Line uno")->text("Line duo")->text("Line tres")->text("")->text("Line Quinto")->text("Lino ses"));
    tui.add(sp);


    return 0;
}

int test_main(int argc, char* argv[])
{
    tui_t tui;
    tui.width(70);
    int hr = 0;
    test(tui);
    hr = tui.build();
    if(hr) return hr;
    fputs(tui.str(), stdout);
    return hr;
}

void usage(char const* const name) NORETURNATTR ;
void usage(char const* const name)
{
    fprintf(stderr, "usage: %s [window_width [window_name]]\n", name);
    exit(255);
}

int exe_main(int argc, char* argv[])
{
    tui_t tui;
    int width = 70;
    char const* name(NULL);
    if(argc > 3) usage(argv[0]);
    if(argc > 1) {
        width = atoi(argv[1]);
        if(!width) usage(argv[0]);
    }
    if(argc > 2) {
        name = argv[2];
    }
    tui.width(width); // TODO from argv
    if(name) tui.name(std::string(name));
    int hr = 0;
    hr = parser(stdin, tui);
    if(hr) return hr;
    hr = tui.build();
    if(hr) return hr;
    fputs(tui.str(), stdout);
    return hr;
}

int main(int argc, char* argv[])
{
    return ENTRYPOINT(argc, argv);
}
