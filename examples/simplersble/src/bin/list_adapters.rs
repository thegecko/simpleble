use simplersble;

fn main() {

    println!(
        "Bluetooth enabled: {}",
        simplersble::InnerAdapter::bluetooth_enabled().unwrap()
    );

    let mut adapters = simplersble::InnerAdapter::get_adapters().unwrap();

    // If the adapter list is empty, print a message and exit
    if adapters.is_empty() {
        println!("No adapters found.");
        return;
    }

    for adapter in adapters.iter_mut() {
        println!("Adapter: {} [{}]", adapter.identifier().unwrap(), adapter.address().unwrap());
    }
}
