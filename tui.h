#ifndef TUI_H
#define TUI_H

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <cstring>

class ComponentDriver;
class AComponent
{
private:
    float m_weight;
public:
    AComponent() : m_weight(1.0f) {}
    AComponent* weight(float const w) { m_weight = w; return this; }
    float my_weight() const { return m_weight; }

    virtual AComponent* add(AComponent const&) { return this; }
    virtual AComponent* text(std::string const&) { return this; }

    virtual std::string str(ComponentDriver const&) const = 0;
    virtual int width(ComponentDriver const&) const = 0;
    virtual int height(ComponentDriver const&) const = 0;
};
struct ComponentDriver
{
    int line;
    int width;
    int height;
};

class Button : public AComponent
{
private:
    std::string m_text;
    std::string m_str;
public:
    virtual AComponent* text(std::string const& text)
    {
        m_text.assign(text);
        return this;
    }
    virtual int width(ComponentDriver const& cd) const
    {
        return 2 + m_text.size() + 2;
    }
    virtual std::string str(ComponentDriver const& cd) const
    {
        std::stringstream s;
        int line = cd.line + 2 - cd.height;

        if(line == height(cd) - 1) {
            s << "| " << m_text << " |";
            return s.str();
        }
        switch(line)
        {
        case 0:
            s << ' ' << std::string(1 + m_text.size() + 1, '_') << ' ';
            return s.str();
        default:
            return std::string(2 + m_text.size() + 2, ' ');
        }
    }
    virtual int height(ComponentDriver const& cd) const
    {
        if(cd.height > 2) return cd.height;
        else return 2;
    }
};

class Label : public AComponent
{
    std::vector<std::string> m_lines;
public:
    virtual AComponent* text(std::string const& text)
    {
        m_lines.push_back(text);
        return this;
    }
    virtual int width(ComponentDriver const& drv) const
    {
        return -1;
    }
    virtual int height(ComponentDriver const& drv) const
    {
        return m_lines.size();
    }
    virtual std::string str(ComponentDriver const& drv) const
    {
        int i = drv.line + height(drv) - drv.height;
        if(i < 0) {
            return std::string(drv.width, ' ');
        } else if(m_lines[i].size() > drv.width) {
            return m_lines[i].substr(0, drv.width);
        } else {
            std::stringstream s;
            s << m_lines[i];
            s << std::string(drv.width - m_lines[i].size(), ' ');
            return s.str();
        }
    }
};

class Combo : public AComponent
{
    std::string m_text;
public:
    virtual AComponent* text(std::string const& text)
    {
        m_text.assign(text);
        return this;
    }
    virtual int width(ComponentDriver const& drv) const
    {
        return -1;
    }
    virtual int height(ComponentDriver const& drv) const
    {
        return 2;
    }
    virtual std::string str(ComponentDriver const& drv) const
    {
        size_t width = drv.width - 2 - 4;
        std::stringstream s;
        int line = drv.line + height(drv) - drv.height;
        switch(line) {
        case 0:
            s << ' ' << std::string(drv.width - 2, '_') << ' ';
            return s.str();
        case 1:
            s << "| ";
            if(width > m_text.size()) {
                s << m_text;
                s << std::string(width - m_text.size(), ' ');
            } else {
                s << m_text.substr(0, width);
            }
            s << " |v|";
            return s.str();
        default:
            return std::string(drv.width, ' ');
        }
    }
};

class MultiLine : public AComponent
{
protected:
    virtual char& GetPaddingChar() const =0;
    std::vector<std::string> m_lines;
public:
    virtual AComponent* text(std::string const& text)
    {
        m_lines.push_back(text);
        return this;
    }
    virtual int width(ComponentDriver const& drv) const
    {
        return -1;
    }
    virtual int height(ComponentDriver const& drv) const
    {
        return std::max(2, (int)(m_lines.size() + 1));
    }
    virtual std::string str(ComponentDriver const& drv) const
    {
        int i = drv.line + height(drv) - drv.height;
        int width = drv.width - 2 - 2;
        bool scrollBar(false);

        if(i < 0) return std::string(drv.width, ' ');

        if(m_lines.size() > 1) {
            width -= 2;
            scrollBar = true;
        }

        std::stringstream s;
        if(i == 0) {
            s << ' ' << std::string(drv.width - 2, '_') << ' ';
            return s.str();
        }

        if(m_lines.size() == 0) {
            s << '|' << std::string(drv.width - 2, '_') << '|';
        }

        if(scrollBar && i != height(drv) - 1) s << "| ";
        else s << "|_";

        if(m_lines[i-1].size() > width) {
            s << m_lines[i-1].substr(0, width);
        } else {
            s << m_lines[i-1];
            if(i == height(drv) - 1) {
                s << std::string(width - m_lines[i-1].size(), '_');
            } else {
                s << std::string(width - m_lines[i-1].size(), GetPaddingChar());
            }
        }

        if(scrollBar) {
            if(i == 1) s << " |^|";
            else if(i == m_lines.size()) s << "_|v|";
            else if(i == 2 || i == 3) s << " |_|";
            else if(i == 2 || i == 3) s << " |_|";
            else s << " | |";
        } else {
            s << "_|";
        }

        return s.str();
    }
};

class TextBox : public MultiLine
{
public:
    virtual char& GetPaddingChar() const
    {
        static char pad = ' ';
        return pad;
    }
};

class ListBox : public MultiLine
{
public:
    virtual char& GetPaddingChar() const
    {
        static char pad = '.';
        return pad;
    }
    virtual int height(ComponentDriver const& drv) const
    {
        return std::max(2, MultiLine::height(drv));
    }
    virtual std::string str(ComponentDriver const& drv) const
    {
        if(m_lines.size() < 1) {
            std::stringstream s;
            switch(drv.line) {
            case 0:
                s << ' ' << std::string(drv.width - 2, '_') << ' ';
                return s.str();
            default:
                s << '|' << std::string(drv.width - 4, '_') << "| |";
                return s.str();
            }
        } else return MultiLine::str(drv);
    }
};

class Menu : public AComponent
{
    std::string m_text;
public:
    virtual AComponent* text(std::string const& text)
    {
        m_text.assign(text);
        return this;
    }
    virtual std::string str(ComponentDriver const&) const
    {
        return m_text;
    }
    virtual int width(ComponentDriver const&) const
    {
        return m_text.length();
    }
    virtual int height(ComponentDriver const&) const
    {
        return 1;
    }
};

/*
class VGroup : public AComponent
{
    std::vector<std::shared_ptr<AComponent> > m_comps;
public:
    virtual int height(ComponentDriver const& drv) const
    {
        int height = 2;
        for(std::vector<std::shared_ptr<AComponent> >::iterator i = m_comps.begin();
                i != m_comps.end(); ++i)
        {
            height += (*i)->height(drv);
        }
        return height;
    }
    virtual int width(ComponentDriver const& drv) const
    {
        int wi = 0;
        bool haveVar(false);
        for(std::vector<std::shared_ptr<AComponent> >::iterator i = m_comps.begin();
                i != m_comps.end(); ++i)
        {
            int w = (*i)->width(drv);
            if(w < 0) haveVar = true;
            if(w > wi) wi = w
        }
        if(haveVar && wi <= drv.width) return -1;
        return wi;
    }
    virtual std::string str(ComponentDriver const& drv) const
    {
        int w = width(drv);

        std::stringstream s;
        switch(drv.line) {
        case 0: s << ' ' << std::string(w - 2, ' ') << ' '; break;
        case 1: s << '|' << 
        }
        return s.str();
    }
};
*/

class Tui
{
    std::vector<std::vector<std::shared_ptr<AComponent> > > m_ctrls;
    size_t m_current;
    std::string m_built;
    int m_width;
    std::string m_name;
    std::vector<std::shared_ptr<AComponent> > m_menus;
public:
    Tui() : m_current(-1), m_width(70), m_name("Window"), m_ctrls(), m_menus() {}
    void add(std::shared_ptr<AComponent> const&);
    void ln();
    int build();
    void width(int const);
    void name(std::string const& name) { m_name = name; }
    char const* const str() const;
};

typedef Tui tui_t;

struct ComponentFactory
{
    static AComponent* New(char const* const s)
    {
        if(strcmp(s, "Label") == 0) return new Label();
        if(strcmp(s, "Button") == 0) return new Button();
        if(strcmp(s, "TextBox") == 0) return new TextBox();
        if(strcmp(s, "ListBox") == 0) return new ListBox();
        if(strcmp(s, "Combo") == 0) return new Combo();
        if(strcmp(s, "Menu") == 0) return new Menu();
        return NULL;
    }
};

#endif
