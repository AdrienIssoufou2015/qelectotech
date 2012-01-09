#include "templatelocationchooser.h"
#include "qetapp.h"
#include "qetproject.h"
#include "templatescollection.h"

/**
	Constructor
	@param location Initial location displayed by the widget
	@param widget Parent QWidget
*/
TitleBlockTemplateLocationChooser::TitleBlockTemplateLocationChooser(
	const TitleBlockTemplateLocation &location,
	QWidget *parent
) :
	QWidget(parent)
{
	init();
	setLocation(location);
}

/**
	Destructor
*/
TitleBlockTemplateLocationChooser::~TitleBlockTemplateLocationChooser() {
}

/**
	@return the current location
*/
TitleBlockTemplateLocation TitleBlockTemplateLocationChooser::location() const {
	return(TitleBlockTemplateLocation(name(), collection()));
}

/**
	@return the currently selected collection
*/
TitleBlockTemplatesCollection *TitleBlockTemplateLocationChooser::collection() const {
	return(collections_index_[collections_ -> currentIndex()]);
}

/**
	@return the currently selected/entered name
*/
QString TitleBlockTemplateLocationChooser::name() const {
	int template_index = templates_ -> currentIndex();
	return(template_index ? templates_ -> currentText() : new_name_ -> text());
}

/**
	Set the location displayed by this widget
	@param location to be displayed by this widget
*/
void TitleBlockTemplateLocationChooser::setLocation(const TitleBlockTemplateLocation &location) {
	// hack: if o suitable index was found, set it to 1, which is supposed to be the user collection
	int index = indexForCollection(location.parentCollection());
	if (index == -1 && collections_ -> count() > 1) index = 1;
	collections_ -> setCurrentIndex(index);
	
	if (!location.name().isEmpty()) {
		int template_index = templates_ -> findText(location.name());
		if (template_index != -1) {
			templates_ -> setCurrentIndex(template_index);
		} else {
			templates_ -> setCurrentIndex(0);
		}
	}
}

/**
	Initialize this widget.
	@param location Initial location displayed by the widget
*/
void TitleBlockTemplateLocationChooser::init() {
	collections_ = new QComboBox();
	templates_ = new QComboBox();
	new_name_ = new QLineEdit();
	
	updateCollections();
	connect(collections_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTemplates()));
	connect(templates_,   SIGNAL(currentIndexChanged(int)), this, SLOT(updateNewName()));
	
	QFormLayout *form_layout = new QFormLayout();
	form_layout -> addRow(tr("Collection parente",   "used in save as form"), collections_);
	form_layout -> addRow(tr("Mod�le existant",      "used in save as form"), templates_);
	form_layout -> addRow(tr("ou nouveau nom",       "used in save as form"), new_name_);
	setLayout(form_layout);
}

/**
	@param coll A Title block templates collection which we want to know the index within the combo box of this dialog.
	@return -1 if the collection is unknown to this dialog, or the index of \a coll
*/
int TitleBlockTemplateLocationChooser::indexForCollection(TitleBlockTemplatesCollection *coll) const {
	QList<int> indexes = collections_index_.keys(coll);
	if (indexes.count()) return(indexes.first());
	return(-1);
}

/**
	Update the collections list
*/
void TitleBlockTemplateLocationChooser::updateCollections() {
	collections_ -> clear();
	collections_index_.clear();
	
	int index = 0;
	foreach(TitleBlockTemplatesCollection *collection, QETApp::availableTitleBlockTemplatesCollections()) {
		collections_ -> addItem(collection -> title());
		collections_index_.insert(index, collection);
		++ index;
	}
	
	updateTemplates();
}

/**
	Update the templates list according to the selected project.
*/
void TitleBlockTemplateLocationChooser::updateTemplates() {
	TitleBlockTemplatesCollection *current_collection = collection();
	if (!current_collection) return;
	
	templates_ -> clear();
	templates_ -> addItem(tr("Nouveau mod�le (entrez son nom)", "combox box entry"), QVariant(false));
	
	QStringList available_templates = current_collection -> templates();
	if (available_templates.count()) {
		templates_ -> insertSeparator(1);
		foreach (QString template_name, available_templates) {
			templates_ -> addItem(template_name, QVariant(true));
		}
	}
	
	updateNewName();
}

/**
	Enable or diable the "new name" text field depending of the selected
	template.
*/
void TitleBlockTemplateLocationChooser::updateNewName() {
	int template_index = templates_ -> currentIndex();
	new_name_ -> setEnabled(!template_index);
}
