#pragma once

#include <hpr/containers/array/dynamic_array.hpp>
#include <memory>


namespace hpr
{

template <typename Type>
class TreeNode
{

public:

    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = Type const*;
    using const_reference = Type const&;

protected:

    pointer p_data;

    darray<TreeNode*> p_descendants;
    TreeNode* p_ancestor;

public:

    friend constexpr
    void swap(TreeNode& main, TreeNode& other) noexcept
    {
        using std::swap;
        swap(main.p_data, other.p_data);
        swap(main.p_descendants, other.p_descendants);
        swap(main.p_ancestor, other.p_ancestor);
    }

    inline
    TreeNode() :
        p_data {new value_type {}},
        p_descendants {},
        p_ancestor {}
    {}

    inline
    TreeNode(const TreeNode& node) :
        p_data {new value_type {*node.p_data}},
        p_descendants {},
        p_ancestor {}
    {}

    inline
    TreeNode(TreeNode&& node)  noexcept :
        p_data {},
        p_descendants {},
        p_ancestor {}
    {
        swap(*this, node);
        /*std::swap(p_data, node.p_data);
        std::swap(p_descendants, node.p_descendants);
        std::swap(p_ancestor, node.p_ancestor);*/
    }

    inline
    TreeNode& operator=(TreeNode other)
    {
        swap(*this, other);
        return *this;
    }

    inline explicit
    TreeNode(const value_type& data) :
        p_data {new value_type {data}},
        p_descendants {},
        p_ancestor {}
    {}

    inline
    TreeNode(const value_type& data, const darray<TreeNode*>& descendants, TreeNode* ancestor) :
        p_data {new value_type {data}},
        p_descendants {descendants},
        p_ancestor {ancestor}
    {
        for (auto descendant : p_descendants)
            descendant->ancestor(this);
    }

    inline
    TreeNode(const value_type& data, const darray<TreeNode*>& descendants) :
        p_data {new value_type {data}},
        p_descendants {descendants},
        p_ancestor {}
    {
        for (auto descendant : p_descendants)
            descendant->ancestor(this);
    }

    virtual
    ~TreeNode()
    {
        delete p_data;
        //delete p_ancestor;
    }

    // Member functions

    inline
    pointer data()
    {
        return p_data;
    }

    inline
    void ancestor(TreeNode* node)
    {
        p_ancestor = node;
    }

    inline
    TreeNode* ancestor()
    {
        return p_ancestor;
    }

    inline
    void descendants(const darray<TreeNode*>& descendants)
    {
        p_descendants = descendants;
    }

    inline
    darray<TreeNode*>& descendants()
    {
        return p_descendants;
    }

    darray<TreeNode*> traverse_descendants()
    {
        std::function<darray<TreeNode*>(TreeNode*)> collect = [&](TreeNode* node)
        {
            darray<TreeNode*> ds;

            if (!node->descendants().is_empty())
            {
                for (TreeNode* dnode: node->descendants())
                {
                    ds.push(dnode);
                    if (!dnode->descendants().is_empty())
                        ds.push(collect(dnode));
                }
            }

            return ds;
        };

        return collect(this);
    }

    darray<TreeNode*> traverse_ancestors()
    {
        std::function<darray<TreeNode*>(TreeNode*)> collect = [&](TreeNode* node)
        {
            darray<TreeNode*> ds;

            if (node->p_ancestor)
            {
                ds.push(node);
                ds.push(collect(node->p_ancestor));
            }

            return ds;
        };

        return collect(this);
    }
};

}