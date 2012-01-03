<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

class HTTPResponse {
	/**
	* Title of the HTTP reponse
	* @var string
	*/
	private $rootElement;
	
	/**
	 * Output data of the HTTP response
	 * @var object
	 */
	private $outputData;
	
	/**
	 * Output type of the HTTP response
	 * @var string
	 */
	private $outputType;
	
	/**
	 * Return headers of the HTTP response
	 * @var string[]
	 */
	private $returnHeaders;
	
	/**
	 * Constructor. Creates a new HTTPResponse object
	 */
	public function __construct() {
	}
	
	/**
	 * Get the title of the HTTP response
	 * @return string
	 */
	public function getRootElement() {
		return $this->rootElement;
	}
	
	/**
	 * Set the title of the HTTP response
	 * @param string $rootElement
	 */
	public function setRootElement($rootElement) {
		$this->rootElement = $rootElement;
	}
	
	/**
	 * Get the output data of the HTTP response
	 * @return object
	 */
	public function getOutputData() {
		return $this->outputData;
	}
	
	/**
	 * Set the output data of the HTTP response
	 * @param object $outputData
	 */
	public function setOutputData($outputData) {
		$this->outputData = $outputData;
	}
	
	/**
	 * Get the output type of the HTTP response
	 * @return string
	 */
	public function getOutputType() {
		return $this->outputType;
	}
	
	/**
	 * Set the output type of the HTTP response
	 * @param string $outputType
	 */
	public function setOutputType($outputType) {
		$this->outputType = $outputType;
	}
	
	/**
	 * Get the return headers of the HTTP response
	 * @return string[]
	 */
	public function getReturnHeaders() {
		return $this->returnHeaders;
	}
	
	/**
	 * Add a return header to the HTTP response
	 * @param string $header
	 */
	public function addReturnHeader($header) {
		$this->returnHeaders[] = $header;
	}
	
	/**
	 * Get the output data of the HTTP response based on the output type
	 * @return string
	 */
	public function getRenderedOutput() {
		switch($this->outputType) {
			case "text/html":
				return sprintf("<h2>%s</h1><pre>%s</pre>", $this->rootElement, print_r($this->outputData, true));
			case "application/json":
				return json_encode(array($this->rootElement => $this->outputData), JSON_NUMERIC_CHECK);
			default:
				return sprintf("%s\n%s", $this->rootElement, print_r($this->outputData, true));
		}
	}
}
?>