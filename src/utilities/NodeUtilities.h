#ifndef _NODE_UTILITIES_H_
#define _NODE_UTILITIES_H_

#include <scenetree/YiSceneNode.h>

// Covers all platforms
#if !(defined YI_DEBUG)
#if defined (DEBUG) || defined (_DEBUG) || !(defined (NDEBUG) || defined (_NDEBUG))
#define YI_DEBUG 1
#endif
#endif

// Used to mark text for translation in situations
// where it isn't possible to use YiTranslate.
#define YI_TRANSLATE_MARKER(text) (text)

namespace Utility
{
    enum GET_NODE_FLAG
    {
        MANDATORY=0,
        OPTIONAL
    };

    enum test
    {
        BLAH,
        BLAH2
    };

    const CYIString TAG("Utility");

    const YI_INT32 IndexedNodeLimit = 100;

    /*!
     * \details Gets a node with the given type, failing with an assertion if the node type cannot be found(if the bOptionalFlag is set to MANDATORY).
     * Note that the name is not required for this method.
     */
    template<typename Type>
    inline Type *GetNode(CYISceneNode *pParent, GET_NODE_FLAG bOptionalFlag = MANDATORY)
    {
        Type *pNode = YI_NULL;

        YI_ASSERT(pParent, TAG, "Parent node is null");
        pNode = pParent->GetNode<Type>();

        if (bOptionalFlag == MANDATORY)
        {
            YI_ASSERT(pNode, TAG, "Missing node from parent: %s", pParent->GetName().GetData());
        }

        return pNode;
    }

    /*!
     * \details Gets a node with the given name and type, failing with an
     * assertion if the node cannot be found(if the bOptionalFlag is set to MANDATORY) or is the wrong type.
     */
    template<typename Type>
    inline Type *GetNode(CYISceneNode *pParent, const CYIString &name, GET_NODE_FLAG bOptionalFlag = MANDATORY)
    {
        Type *pNode = YI_NULL;

        if (!name.IsEmpty())
        {
            YI_ASSERT(pParent, TAG, "Parent node is null");
            pNode = pParent->GetNode<Type>(name);

            if(bOptionalFlag == MANDATORY)
            {
                YI_ASSERT(pNode, TAG, "Missing node: %s", name.GetData());
            }
        }

        return pNode;
    }

    /*!
     * \details Gets a node with the given name, failing
     * with an assertion if the node cannot be found if the bOptionalFlag is set to MANDATORY.
     */
    inline CYISceneNode *GetNode(CYISceneNode *pParent, const CYIString &name, GET_NODE_FLAG bOptionalFlag = MANDATORY)
    {
        return GetNode<CYISceneNode>(pParent, name, bOptionalFlag);
    }

    /*!
        \details Returns a vector of nodes of the given type whose names match
        the given name suffixed with an integral index starting at 0. Given the
        name "Node", this function would return a vector of nodes matching the
        names "Node0", "Node1", "Node2", etc. up to a limit of 100 nodes. The
        first missing node stops the search for nodes.
    */
    template<typename Type>
    inline std::vector<Type *> GetIndexedNodes(CYISceneNode *pParent, const CYIString &name)
    {
        std::vector<Type *> nodes;

        if (!name.IsEmpty())
        {
            YI_ASSERT(pParent, TAG, "Parent node is null");

            for (YI_INT32 nIndex = 0; nIndex < IndexedNodeLimit; ++nIndex)
            {
                if (Type *pNode = pParent->GetNode<Type>(name + CYIString::FromValue(nIndex)))
                {
                    nodes.push_back(pNode);
                }
                else
                {
                    break;
                }
            }
        }

        return nodes;
    }

    /*!
        \details Returns a vector of nodes type whose names match the given
        name suffixed with an integral index starting at 0. Given the name
        "Node", this function would return a vector of nodes matching the names
        "Node0", "Node1", "Node2", etc. up to a limit of 100 nodes. The first
        missing node stops the search for nodes.
    */
    inline std::vector<CYISceneNode *> GetIndexedNodes(CYISceneNode *pParent, const CYIString &name)
    {
        return GetIndexedNodes<CYISceneNode>(pParent, name);
    }

    /*!
        \details Returns the property with the given name or false if
        the property is missing or can't be converted to the given type.
    */
    template<typename Type>
    inline bool GetProperty(const CYISceneNode *pNode, const CYIString &propertyName, Type &propertyValue)
    {
        YI_ASSERT(pNode, TAG, "Node is null");

        CYIString propertyString;
        if (pNode->GetProperty(propertyName, &propertyString) && propertyString.IsNotEmpty())
        {
            bool bError = false;
            propertyValue = propertyString.To<Type>(&bError);
            return !bError;
        }
        return false;
    }

    /*!
        \details A specialization of template<typename Type>GetProperty() for bool.
    */
    template<>
    inline bool GetProperty<bool>(const CYISceneNode *pNode, const CYIString &propertyName, bool &propertyValue)
    {
        YI_ASSERT(pNode, TAG, "Node is null");

        CYIString propertyString;
        if (pNode->GetProperty(propertyName, &propertyString) && propertyString.IsNotEmpty())
        {
            propertyValue = propertyString == "true";
            return true;
        }
        return false;
    }

    /*!
        \details A specialization of template<typename Type>GetProperty() for CYIString.
    */
    template<>
    inline bool GetProperty<CYIString>(const CYISceneNode *pNode, const CYIString &propertyName, CYIString &propertyValue)
    {
        YI_ASSERT(pNode, TAG, "Node is null");

        if (pNode->GetProperty(propertyName, &propertyValue) && propertyValue.IsNotEmpty())
        {
            return true;
        }
        return false;
    }

}

#endif // _NODE_UTILITIES_H_

