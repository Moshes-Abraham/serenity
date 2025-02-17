/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefPtr.h>
#include <LibJS/Heap/GCPtr.h>
#include <LibWeb/CSS/Display.h>
#include <LibWeb/CSS/Selector.h>
#include <LibWeb/Forward.h>

namespace Web::Layout {

class TreeBuilder {
public:
    TreeBuilder();

    JS::GCPtr<Layout::Node> build(DOM::Node&);

private:
    struct Context {
        bool has_svg_root = false;
    };

    ErrorOr<void> create_layout_tree(DOM::Node&, Context&);

    void push_parent(Layout::NodeWithStyle& node) { m_ancestor_stack.append(node); }
    void pop_parent() { m_ancestor_stack.take_last(); }

    template<CSS::Display::Internal, typename Callback>
    void for_each_in_tree_with_internal_display(NodeWithStyle& root, Callback);

    template<CSS::Display::Inside, typename Callback>
    void for_each_in_tree_with_inside_display(NodeWithStyle& root, Callback);

    void fixup_tables(NodeWithStyle& root);
    void remove_irrelevant_boxes(NodeWithStyle& root);
    void generate_missing_child_wrappers(NodeWithStyle& root);
    Vector<JS::Handle<Box>> generate_missing_parents(NodeWithStyle& root);
    void missing_cells_fixup(Vector<JS::Handle<Box>> const&);

    enum class AppendOrPrepend {
        Append,
        Prepend,
    };
    void insert_node_into_inline_or_block_ancestor(Layout::Node&, CSS::Display, AppendOrPrepend);
    ErrorOr<void> create_pseudo_element_if_needed(DOM::Element&, CSS::Selector::PseudoElement, AppendOrPrepend);

    JS::GCPtr<Layout::Node> m_layout_root;
    Vector<JS::NonnullGCPtr<Layout::NodeWithStyle>> m_ancestor_stack;
};

}
