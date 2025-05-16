use std::pin::Pin;
use std::mem;

use super::ffi;
use crate::peripheral::Peripheral;
use crate::types::Error;

pub struct Adapter {
    internal: cxx::UniquePtr<ffi::RustyAdapter>,
    on_scan_start: Box<dyn Fn() + Send + Sync + 'static>,
    on_scan_stop: Box<dyn Fn() + Send + Sync + 'static>,
    on_scan_found: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>,
    on_scan_updated: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>,
}

impl Adapter {
    pub fn bluetooth_enabled() -> Result<bool, Error> {
        ffi::RustyAdapter_bluetooth_enabled().map_err(Error::from_cxx_exception)
    }

    pub fn get_adapters() -> Result<Vec<Pin<Box<Adapter>>>, Error> {
        let mut raw_adapter_list = ffi::RustyAdapter_get_adapters().map_err(Error::from_cxx_exception)?;

        let mut adapters = Vec::<Pin<Box<Adapter>>>::new();
        for adapter_wrapper in raw_adapter_list.iter_mut() {
            adapters.push(Adapter::new(adapter_wrapper));
        }
        Ok(adapters)
    }

    fn new(wrapper: &mut ffi::RustyAdapterWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyAdapter>::null(),
            on_scan_start: Box::new(|| {}),
            on_scan_stop: Box::new(|| {}),
            on_scan_found: Box::new(|_| {}),
            on_scan_updated: Box::new(|_| {}),
        };

        let mut this_boxed = Box::pin(this);
        wrapper.internal.link(this_boxed.as_mut()).unwrap();
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn identifier(&self) -> Result<String, Error> {
        self.internal.identifier().map_err(Error::from_cxx_exception)
    }

    pub fn address(&self) -> Result<String, Error> {
        self.internal.address().map_err(Error::from_cxx_exception)
    }

    pub fn scan_start(&self) -> Result<(), Error> {
        self.internal.scan_start().map_err(Error::from_cxx_exception)
    }

    pub fn scan_stop(&self) -> Result<(), Error> {
        self.internal.scan_stop().map_err(Error::from_cxx_exception)
    }

    pub fn scan_for(&self, timeout_ms: i32) -> Result<(), Error> {
        self.internal.scan_for(timeout_ms).map_err(Error::from_cxx_exception)
    }

    pub fn scan_is_active(&self) -> Result<bool, Error> {
        self.internal.scan_is_active().map_err(Error::from_cxx_exception)
    }

    pub fn scan_get_results(&self) -> Result<Vec<Pin<Box<Peripheral>>>, Error> {
        let mut raw_peripheral_list = self.internal.scan_get_results().map_err(Error::from_cxx_exception)?;

        let mut peripherals = Vec::<Pin<Box<Peripheral>>>::new();
        for peripheral_wrapper in raw_peripheral_list.iter_mut() {
            peripherals.push(Peripheral::new(peripheral_wrapper));
        }

        return Ok(peripherals);
    }

    pub fn get_paired_peripherals(&self) -> Result<Vec<Pin<Box<Peripheral>>>, Error> {
        let mut raw_peripheral_list =
            self.internal.get_paired_peripherals().map_err(Error::from_cxx_exception)?;

        let mut peripherals = Vec::<Pin<Box<Peripheral>>>::new();
        for peripheral_wrapper in raw_peripheral_list.iter_mut() {
            peripherals.push(Peripheral::new(peripheral_wrapper));
        }

        return Ok(peripherals);
    }

    pub fn set_callback_on_scan_start(&mut self, cb: Box<dyn Fn() + Send + Sync + 'static>) {
        self.on_scan_start = cb;
    }

    pub fn set_callback_on_scan_stop(&mut self, cb: Box<dyn Fn() + Send + Sync + 'static>) {
        self.on_scan_stop = cb;
    }

    pub fn set_callback_on_scan_updated(
        &mut self,
        cb: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>,
    ) {
        self.on_scan_updated = cb;
    }

    pub fn set_callback_on_scan_found(
        &mut self,
        cb: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>,
    ) {
        self.on_scan_found = cb;
    }

    pub fn on_callback_scan_start(&self) {
        (self.on_scan_start)();
    }

    pub fn on_callback_scan_stop(&self) {
        (self.on_scan_stop)();
    }

    pub fn on_callback_scan_updated(&self, peripheral: &mut ffi::RustyPeripheralWrapper) {
        (self.on_scan_updated)(Peripheral::new(peripheral));
    }

    pub fn on_callback_scan_found(&self, peripheral: &mut ffi::RustyPeripheralWrapper) {
        (self.on_scan_found)(Peripheral::new(peripheral));
    }
}

impl Drop for Adapter {
    fn drop(&mut self) {
        self.internal.unlink().unwrap();
    }
}

unsafe impl Sync for Adapter {}
unsafe impl Send for Adapter {}