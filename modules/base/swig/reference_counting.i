%{

#define _SWIG_ref_count "_SWIG_ref_count"

inline void _SWIG_ma_Node_reset(ma::Node* node, const ma::Any& value, bool recursive = true)
{
  if (node == nullptr) return;
  node->setProperty(_SWIG_ref_count, value);
  if (recursive)
  {
    auto& children = node->children();
    for (auto child : children)
      _SWIG_ma_Node_reset(child, value, recursive);
  }
};

inline void _SWIG_ma_Node_ref(ma::Node* node)
{
  if (node == nullptr) return;
  const auto& prop = node->property(_SWIG_ref_count);
  if (prop.isValid())
    node->setProperty(_SWIG_ref_count, prop.cast<int>() + 1);
  else
    node->setProperty(_SWIG_ref_count, static_cast<int>(node->parents().size()));
/*  mexPrintf("_SWIG_ma_Node_ref - %s - new count: %i\n",node->name().c_str(), rc.cast<int>());*/
};

inline int _SWIG_ma_Node_unref(ma::Node* node)
{
  if (node == nullptr) return 0;
  int rc = node->property(_SWIG_ref_count).cast<int>();
  rc -= 1;
/*  mexPrintf("_SWIG_ma_Node_unref - %s - remaining: %i\n", node->name().c_str(), rc);*/
  if (rc < 0)
  {
    auto children = node->children();
    for (auto child : children)
    {
      child->removeParent(node);
      _SWIG_ma_Node_unref(child);
    }
/*    mexPrintf("_SWIG_ma_Node_unref - %s - ~ma::Node (children number: %i)\n", node->name().c_str(), node->children().size());*/
    delete node;
  }
  else
    node->setProperty(_SWIG_ref_count, rc);
  return rc;
};

%};