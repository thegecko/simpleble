use std::sync::Arc;
use std::pin::Pin;
use std::mem;

use super::ffi;
use crate::characteristic::InnerCharacteristic;
use crate::characteristic::PublicCharacteristic;
pub struct InnerService {
    internal: cxx::UniquePtr<ffi::RustyService>,
}

impl InnerService {
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

    pub fn characteristics(&self) -> Vec<Pin<Box<InnerCharacteristic>>> {
        // TODO: Remove once full migration to public classes is done.
        let mut characteristics = Vec::<Pin<Box<InnerCharacteristic>>>::new();

        for characteristic_wrapper in self.internal.characteristics().iter_mut() {
            characteristics.push(InnerCharacteristic::new(characteristic_wrapper));
        }

        return characteristics;
    }

    pub fn public_characteristics(&self) -> Vec<PublicCharacteristic> {
        let mut characteristics = Vec::<PublicCharacteristic>::new();

        for characteristic_wrapper in self.internal.characteristics().iter_mut() {
            characteristics.push(InnerCharacteristic::new(characteristic_wrapper).into());
        }

        return characteristics;
    }
}

unsafe impl Sync for InnerService {}
unsafe impl Send for InnerService {}

#[derive(Clone)]
pub struct PublicService {
    inner: Arc<Pin<Box<InnerService>>>,
}

impl PublicService {
    pub fn uuid(&self) -> String {
        return self.inner.uuid();
    }

    pub fn data(&self) -> Vec<u8> {
        return self.inner.data();
    }

    pub fn characteristics(&self) -> Vec<PublicCharacteristic> {
        return self.inner.public_characteristics();
    }

}

impl From<Pin<Box<InnerService>>> for PublicService {
    fn from(service: Pin<Box<InnerService>>) -> Self {
        return PublicService {
            inner: Arc::new(service),
        };
    }
}

unsafe impl Send for PublicService {}
unsafe impl Sync for PublicService {}
