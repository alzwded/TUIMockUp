#include "tui.h"
#include <assert.h>
#include <climits>

void Tui::add(std::shared_ptr<AComponent> const& c)
{
    m_built.assign("");
    assert(m_current < m_ctrls.size());
    m_ctrls[m_current].push_back(c);
}

void Tui::ln()
{
    m_built.assign("");
    m_ctrls.resize(m_ctrls.size() + 1);
    m_current++;
}

void Tui::width(int const w)
{
    m_width = w;
}

int Tui::build()
{
    int max_width = m_width - 1 - 2;
    for(size_t i = 0; i < m_ctrls.size(); ++i) {
        int line_width = 3;
        ComponentDriver drv;
        drv.line = 0;
        if(m_ctrls[i].size() == 0) continue;
        drv.width = max_width / m_ctrls[i].size();
        drv.height = 1;
        for(size_t j = 0; j < m_ctrls[i].size(); ++j) {
            int w = m_ctrls[i][j]->width(drv);
            if(w < 0) line_width++;
            else line_width += 1 + w;
        }
        if(line_width > max_width) max_width = line_width;
    }

    std::stringstream s;

    // title bar
    s << ' ' << std::string(max_width - 2, '_') << ' ' << std::endl;
    s << "|X| " << m_name << std::string(max_width - 4 - m_name.size() - 4, ' ') << " |+|" << std::endl;
    s << '|' << std::string(max_width - 2, '-') << '|' << std::endl;

    for(size_t i = 0; i < m_ctrls.size(); ++i) {
        int max_height = 0;
        ComponentDriver drv;
        if(m_ctrls[i].size() == 0) {
            s << "|" << std::string(max_width - 2, ' ') << "|" << std::endl;
            continue;
        }
        drv.width = max_width / m_ctrls[i].size();
        drv.line = 0;
        drv.height = 1;
        for(size_t j = 0; j < m_ctrls[i].size(); ++j) {
            int h = m_ctrls[i][j]->height(drv);
            if(h > max_height) max_height = h;
        }
        drv.height = max_height;

        // compute width of variable width components
        std::vector<int> widths(m_ctrls[i].size(), 0);
        float max_weight = 0.0f;
        int remaining = max_width - 3;
        bool haveVariableWidthComponents(false);
        for(size_t j = 0; j < m_ctrls[i].size(); ++j) {
            remaining--;
            int w = m_ctrls[i][j]->width(drv);
            if(w > 0) {
                widths[j] = w;
                remaining -= w;
            } else {
                max_weight += m_ctrls[i][j]->my_weight();
                haveVariableWidthComponents = true;
            }
        }
        int splitremaining = remaining;
        struct {
            size_t idx;
            int val;
        } min = { -1, INT_MAX };
        if(haveVariableWidthComponents)
        for(size_t j = 0; j < m_ctrls[i].size(); ++j) {
            int w = m_ctrls[i][j]->width(drv);
            if(w <= 0) {
                widths[j] = splitremaining * m_ctrls[i][j]->my_weight() / max_weight;
                if(widths[j] < min.val) {
                    min.idx = j;
                    min.val = widths[j];
                }
                remaining -= widths[j];
            }
        }
        if(haveVariableWidthComponents && remaining) {
            widths[min.idx] += remaining;
            remaining = 0;
        }
        for(int h = 0; h < drv.height; ++h) {
            drv.line = h;
            s << "|";
            for(size_t j = 0; j < m_ctrls[i].size(); ++j) {
                drv.width = widths[j];
                s << " " << m_ctrls[i][j]->str(drv);
            }
            if(remaining) {
                s << std::string(remaining, ' ');
            }
            s << " |" << std::endl;
        }
    }

    s << "|" << std::string(max_width - 2, '_') << "|" << std::endl;

    m_built.assign(s.str());

    return 0;
}

char const* const Tui::str() const
{
    return m_built.c_str();
}
