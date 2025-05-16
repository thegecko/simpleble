use std::pin::Pin;
use std::mem;

use super::ffi;
use crate::characteristic::Characteristic;

pub struct Service {
    internal: cxx::UniquePtr<ffi::RustyService>,
}

impl Service {
    pub(crate) fn new(wrapper: &mut ffi::RustyServiceWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyService>::null(),
        };

        let mut this_boxed = Box::pin(this);
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn uuid(&self) -> String {
        return self.internal.uuid();
    }

    pub fn data(&self) -> Vec<u8> {
        return self.internal.data();
    }

    pub fn characteristics(&self) -> Vec<Pin<Box<Characteristic>>> {
        let mut characteristics = Vec::<Pin<Box<Characteristic>>>::new();

        for characteristic_wrapper in self.internal.characteristics().iter_mut() {
            characteristics.push(Characteristic::new(characteristic_wrapper));
        }

        return characteristics;
    }
}

unsafe impl Sync for Service {}
unsafe impl Send for Service {}