use std::sync::Arc;
use std::pin::Pin;
use std::mem;

use super::ffi;

pub struct Descriptor {
    internal: cxx::UniquePtr<ffi::RustyDescriptor>,
}

impl Descriptor {
    pub(crate) fn new(wrapper: &mut ffi::RustyDescriptorWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyDescriptor>::null(),
        };

        let mut this_boxed = Box::pin(this);
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn uuid(&self) -> String {
        return self.internal.uuid();
    }
}

unsafe impl Sync for Descriptor {}
unsafe impl Send for Descriptor {}

#[derive(Clone)]
pub struct PublicDescriptor {
    inner: Arc<Pin<Box<Descriptor>>>,
}

impl PublicDescriptor {
    // TODO: Decide how to implement `new`

    pub fn uuid(&self) -> String {
        return self.inner.uuid();
    }
}


impl From<Pin<Box<Descriptor>>> for PublicDescriptor {
    fn from(descriptor: Pin<Box<Descriptor>>) -> Self {
        return PublicDescriptor {
            inner: Arc::new(descriptor),
        };
    }
}

unsafe impl Send for PublicDescriptor {}
unsafe impl Sync for PublicDescriptor {}
