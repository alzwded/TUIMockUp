#include "parser.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <memory>
#include "tui.h"

int parser(FILE* f, tui_t & tui)
{
    char buf[4096];
    std::shared_ptr<AComponent> sp;
    tui.ln();
    while(!feof(f)) {
        fgets(buf, sizeof(buf), f);
        if(buf[0] == '\0') return 0;
        buf[strlen(buf) - 1] = '\0';
        if(buf[0] == '\0') return 0;
        char* p = strtok(buf, " ");
        if(p == NULL) abort();
        sp.reset(ComponentFactory::New(p));
        if(!sp && (strcmp(p, "LN") == 0 || strcmp(p, "_") == 0)) {
            tui.ln();
            continue;
        }
        assert(sp);
        p = strtok(NULL, " ");
        if(p) {
            float weight = atof(p);
            sp->weight(weight);
        }
        std::string txt;

readNewLine:
        fgets(buf, sizeof(buf), f);
        buf[strlen(buf) - 1] = '\0';
        if(buf[0] == '\0') goto AddToTUI;
        txt.assign(buf);
        sp->text(txt);
        goto readNewLine;

AddToTUI:
        tui.add(sp);
    }

    return 0;
}
