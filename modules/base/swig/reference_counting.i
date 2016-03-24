%{

#define _MA_REF_COUNTER "_MA_REF_COUNTER"

inline ma::Any _ma_refcount_set(ma::Node* node)
{
  return node->property(_MA_REF_COUNTER);
};

inline void _ma_refcount_set(ma::Node* node, const ma::Any& value)
{
  node->setProperty(_MA_REF_COUNTER, value);
};

inline void _ma_refcount_reset(ma::Node* node, const ma::Any& value, bool recursive = true)
{
  _ma_refcount_set(node, value);
  if (recursive)
  {
    auto& children = node->children();
    for (auto child : children)
      _ma_refcount_reset(child, value, recursive);
  }
};

inline void _ma_refcount_incr(ma::Node* node)
{
  const auto& prop = _ma_refcount_set(node);
  if (prop.isValid())
    _ma_refcount_set(node, static_cast<int>(prop) + 1);
  else
    _ma_refcount_set(node, static_cast<int>(node->parents().size()));
};

inline int _ma_refcount_decr(ma::Node* node)
{
  int count = _ma_refcount_set(node);
  count -= 1;
  if (count < 0)
  {
    auto children = node->children();
    for (auto child : children)
    {
      child->removeParent(node);
      _ma_refcount_decr(child);
    }
/*    mexPrintf("_ma_refcount_decr - %s - ~ma::Node (children number: %i)\n", node->name().c_str(), node->children().size());*/
    delete node;
  }
  else
    _ma_refcount_set(node, count);
  return count;
};

%};