#ifndef _TABBED_VIEW_PAGE_CONTROLLER_INL_H_
#define _TABBED_VIEW_PAGE_CONTROLLER_INL_H_

template<typename ValueType>
void TabbedViewPageController::AugmentBundleOnPreloadWithData(const CYIString &key, const ValueType &value)
{
    struct ConcreteBundleAugmentor : public BundleAugmentor
    {
        CYIString m_key;
        ValueType m_value;

        ConcreteBundleAugmentor(const CYIString &key, const ValueType &value)
            : m_key(key)
            , m_value(value)
        {
        }

        virtual void AugmentBundle(CYIBundle &bundle) const
        {
            bundle.Put(m_key, m_value);
        }
    };

    m_bundleAugmentors.push_back(new ConcreteBundleAugmentor(key, value));
}

#endif // _TABBED_VIEW_PAGE_CONTROLLER_INL_H_

