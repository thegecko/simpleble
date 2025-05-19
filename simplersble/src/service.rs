use std::sync::Arc;
use std::pin::Pin;
use std::mem;

use super::ffi;
use crate::characteristic::Characteristic;
use crate::characteristic::PublicCharacteristic;
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
        // TODO: Remove once full migration to public classes is done.
        let mut characteristics = Vec::<Pin<Box<Characteristic>>>::new();

        for characteristic_wrapper in self.internal.characteristics().iter_mut() {
            characteristics.push(Characteristic::new(characteristic_wrapper));
        }

        return characteristics;
    }

    pub fn public_characteristics(&self) -> Vec<PublicCharacteristic> {
        let mut characteristics = Vec::<PublicCharacteristic>::new();

        for characteristic_wrapper in self.internal.characteristics().iter_mut() {
            characteristics.push(Characteristic::new(characteristic_wrapper).into());
        }

        return characteristics;
    }
}

unsafe impl Sync for Service {}
unsafe impl Send for Service {}

#[derive(Clone)]
pub struct PublicService {
    inner: Arc<Pin<Box<Service>>>,
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

impl From<Pin<Box<Service>>> for PublicService {
    fn from(service: Pin<Box<Service>>) -> Self {
        return PublicService {
            inner: Arc::new(service),
        };
    }
}

unsafe impl Send for PublicService {}
unsafe impl Sync for PublicService {}
