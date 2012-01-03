<?php
/*

This file is part of CaptureTheData!

Copyright (c) 2011, Bobbie Smulders

Contact:  mail@bsmulders.com

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

*/

class HTTPRequest {
	/**
	 * Hostname of the HTTP request
	 * @var string
	 */
	private $host;
	
	/**
	 * URI of the HTTP request
	 * @var string
	 */
	private $uri;
	
	/**
	 * Method of the HTTP request
	 * @var string
	 */
	private $method;
	
	/**
	 * HTTP Accept header of the HTTP request
	 * @var string[]
	 */
	private $httpAccepts;
	
	/**
	 * Constructor. Creates a new HTTPRequest object using the PHP server and 
	 * execution environment information.
	 * @param object[] $server
	 */
	public function __construct($server) {
		$this->uri = strtok($server['REQUEST_URI'], '?');
		$this->host = $server['HTTP_HOST'];
		$this->method = $server['REQUEST_METHOD'];
		$this->httpAccepts = explode(",", $server['HTTP_ACCEPT']);
	}
	
	/**
	 * Get the hostname of the HTTP request
	 * @return string
	 */
	public function getHost() {
		return $this->host;
	}
	
	/**
	 * Get the URI of the HTTP request
	 * @return string
	 */
	public function getUri() {
		return $this->uri;
	}
	
	/**
	 * Get the method of the HTTP request
	 * @return string
	 */
	public function getMethod() {
		return $this->method;
	}
	
	/**
	 * Get the HTTP Accept headers of the HTTP request
	 * @return string[]
	 */
	public function getHttpAccepts() {
		return $this->httpAccepts;
	}
	
	/**
	 * Get the first HTTP Accept header that is also in the acceptable array
	 * @param string[]
	 * @return string
	 */
	public function getHttpAccept($acceptable) {
		$intersect = array_intersect($acceptable, $this->httpAccepts);
		sort($intersect);
		return (sizeof($intersect) > 0) ? $intersect[0] : null;
	}
}
?>