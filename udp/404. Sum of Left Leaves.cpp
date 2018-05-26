/*
 *
 *Find the sum of all left leaves in a given binary tree.
 *
 *Example:
 *
 *    3
 *   / \
 *  9  20
 *    /  \
 *   15   7
 *
 *There are two left leaves in the binary tree, with values 9 and 15 respectively. Return 24.
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    void handler(TreeNode* root, int& sum, bool isLeftChild)
    {
        if (!root)
            return;
        if (!root->left && !root->right && isLeftChild)
        {
            sum += root->val;
        }

        handler(root->left, sum, true);
        handler(root->right, sum, false);
    }


    int sumOfLeftLeaves(TreeNode* root)
    {
        if (!root)
            return 0;
        int sum = 0;
        handler(root->left, sum, true);
        handler(root->right, sum, false);

        return sum;
    }
};
