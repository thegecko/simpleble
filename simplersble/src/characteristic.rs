use std::pin::Pin;
use std::mem;

use super::ffi;
use crate::descriptor::Descriptor;
use crate::types::CharacteristicCapability;

pub struct Characteristic {
    internal: cxx::UniquePtr<ffi::RustyCharacteristic>,
}

impl Characteristic {
    pub(crate) fn new(wrapper: &mut ffi::RustyCharacteristicWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyCharacteristic>::null(),
        };

        let mut this_boxed = Box::pin(this);
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn uuid(&self) -> String {
        return self.internal.uuid();
    }

    pub fn descriptors(&self) -> Vec<Pin<Box<Descriptor>>> {
        let mut descriptors = Vec::<Pin<Box<Descriptor>>>::new();

        for descriptor_wrapper in self.internal.descriptors().iter_mut() {
            descriptors.push(Descriptor::new(descriptor_wrapper));
        }

        return descriptors;
    }

    pub fn capabilities(&self) -> Vec<CharacteristicCapability> {
        let mut capabilities = Vec::<CharacteristicCapability>::new();

        if self.internal.can_read() {
            capabilities.push(CharacteristicCapability::Read);
        }

        if self.internal.can_write_request() {
            capabilities.push(CharacteristicCapability::WriteRequest);
        }

        if self.internal.can_write_command() {
            capabilities.push(CharacteristicCapability::WriteCommand);
        }

        if self.internal.can_notify() {
            capabilities.push(CharacteristicCapability::Notify);
        }

        if self.internal.can_indicate() {
            capabilities.push(CharacteristicCapability::Indicate);
        }

        return capabilities;
    }

    pub fn can_read(&self) -> bool {
        return self.internal.can_read();
    }

    pub fn can_write_request(&self) -> bool {
        return self.internal.can_write_request();
    }

    pub fn can_write_command(&self) -> bool {
        return self.internal.can_write_command();
    }

    pub fn can_notify(&self) -> bool {
        return self.internal.can_notify();
    }

    pub fn can_indicate(&self) -> bool {
        return self.internal.can_indicate();
    }
}

unsafe impl Sync for Characteristic {}
unsafe impl Send for Characteristic {}