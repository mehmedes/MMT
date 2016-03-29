package eu.modernmt.processing.framework.string;

import eu.modernmt.model.Token;

import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;

/**
 * Created by lucamastrostefano on 25/03/16.
 */
public class StringEditor {

    private List<ProcessedString.Operation> changeLog;
    private TreeSet<Token> tokens;
    private ProcessedString processedString;
    private int lastEditedIndex;
    private int deltaIndexes;
    private boolean inUse;

    protected StringEditor(ProcessedString processedString) {
        this.processedString = processedString;
    }

    protected void init() {
        this.changeLog = new LinkedList<ProcessedString.Operation>();
        this.tokens = new TreeSet<Token>(this.processedString.getTokens());
        this.lastEditedIndex = 0;
        this.deltaIndexes = 0;
        this.inUse = true;
    }

    public String getOriginalString() {
        return processedString.getCurrentString();
    }

    public void replace(int startIndex, int length, String string) {
        if (startIndex > this.lastEditedIndex) {
            ProcessedString.Operation operation = new ProcessedString.Operation();
            operation.startIndex = startIndex + this.deltaIndexes;
            operation.length = length;
            operation.lengthNewString = string.length();
            operation.newString = string;
            this.changeLog.add(operation);
            this.lastEditedIndex = startIndex + length;
            this.deltaIndexes += (operation.lengthNewString - operation.length);
        } else {
            throw new RuntimeException("Overlapping operation");
        }
    }

    public void delete(int startIndex, int length) {
        this.replace(startIndex, length, "");
    }

    public void insert(int startIndex, String string) {
        this.replace(startIndex, 0, string);
    }

    public void setWord(int startIndex, int length, String string) {
        //Replace and create token
    }

    public void setXMLTag(int startIndex, int length) {
        //Replace with "" and create tag
    }

    public void submitChanges() {
        this.processedString.applyOperations(this.changeLog);
        this.processedString = null;
        this.changeLog = null;
        this.tokens = null;
        this.inUse = false;
    }

    public void discardChanges() {
        this.processedString = null;
        this.changeLog = null;
        this.tokens = null;
        this.inUse = false;
    }

    public boolean isInUse() {
        return inUse;
    }
}
