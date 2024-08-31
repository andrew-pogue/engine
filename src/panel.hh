#pragma once
#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>
#include "widget.hh"

// represents a rectangular area of the screen, can contain other widgets
struct Panel : Widget {
public:

    Panel(float w, float h)
        : Widget(w, h)
        , flags{true, true}
        , widgets_()
    {}

    virtual ~Panel() {}

          Widget &top()       { return widgets_.back(); }
    const Widget &top() const { return widgets_.back(); }

    void pop() { widgets_.pop_back(); }
    // std::shared_ptr<Widget> pop() { auto w = widgets_.back(); widgets_.pop_back(); return w; }
    // void push(std::shared_ptr<Widget> widget) { widgets_.push_back(widget); }
 
    template <typename W, typename... Args>
    void push(Args... args) {
        assert(std::is_base_of_v<Widget, W> && "[Panel] attempted to push non-widget type");
        widgets_.emplace_back(new W(args...));
    }

    // returns false if none of the panels widgets handled the event
    //      indicating that the caller should handle the event instead
    bool handle_event(const ALLEGRO_EVENT &event) override {
        bool is_handled = false;
        for (size_t i = widgets_.size();
            i > 0ull && !(is_handled = widgets_[i].handle_event(event));
            i--);
        return flags.trap_events || is_handled;
    }

    // returns false if an exit condition was met by any widget
    //      indicating that the panel should close
    bool update(double dt) override {
        bool status = true;
        for (size_t i = widgets_.size()-1; i >= 0ull; i--)
            status = status && widgets_[i].update(dt);
        return status;
    }

    virtual void render(float x, float y) const override {
        auto clip = ScopedClippingRectangle(x, y, w, h);
        for (auto &widget : widgets_) widget->render();
    }

    virtual void layout() = 0;

    struct Flags {
        // determines whether unhandled events can bleed through to panels behind this one
        bool trap_events : 1;
        // detemerines whether panels behind this would should be rendered or not
        bool trap_render : 1;
    } flags;

protected:

    void swap(size_t i, size_t j) {
        assert(i < widgets_.size() && j < widgets_.size() && "[Panel] index out of range");
        auto ival = widgets_[i];
        widgets_[i] = widgets_[j];
        widgets_[j] = ival;
    }

private:

    // the end of the vector is the top of the stack
    std::vector<std::shared_ptr<Widget>> widgets_;

};

